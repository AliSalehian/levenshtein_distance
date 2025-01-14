#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <malloc.h>

#define MEM_ERROR -1

static PyObject *lev_dis_error;

static unsigned char minimum(unsigned char a, unsigned char b, unsigned char c) {
    if (a < b) {
        if (a < c)
            return a;
        return c;
    }
    if (b < c)
        return b;
    return c;
}

static unsigned char lev_dist_single(unsigned char *s1, unsigned char *s2, size_t s1_size, size_t s2_size)
{
    unsigned char **d = NULL;
    unsigned char dist;
    unsigned char cost = 0;

    if (s1_size == 0 && s2_size == 0)
        return (unsigned char)0;
    if (s1_size == 0)
        return s2_size;
    if (s2_size == 0)
        return s1_size;

    d = (unsigned char**) malloc((s2_size+1) * sizeof(unsigned char*));
    if (d == NULL) {
        perror("Failed to allocate memory!\n");
        return MEM_ERROR;
    }
    for (size_t i=0; i < s2_size+1; i++) {
        d[i] = (unsigned char*) calloc((s1_size+1), sizeof(unsigned char));
        if (d[i] == NULL) {
            perror("Failed to allocate memory!\n");
            return MEM_ERROR;
        }
    }

    for (size_t i=1; i <= s2_size; i++)
        d[i][0] = i;
    for (size_t i=1; i <= s1_size; i++)
        d[0][i] = i;

    for (size_t j=1; j <= s1_size; j++) {
        for (size_t i=1; i <= s2_size; i++) {
            cost = s1[j-1] == s2[i-1] ? 0 : 1;
            d[i][j] = minimum(d[i-1][j]+1, d[i][j-1]+1, d[i-1][j-1] + cost);
        }
    }
    dist = d[s2_size][s1_size];
    for (size_t i=0; i < s2_size+1; i++)
        free(d[i]);
    free(d);
    return dist;
}

static PyObject *lev_dist_single_py(PyObject *self, PyObject *args)
{
    PyObject *input1 = NULL, *input2 = NULL;
    unsigned char *array1 = NULL, *array2 = NULL;
    unsigned char result;
    Py_ssize_t len1 = 0, len2 = 0;

    if (!PyArg_ParseTuple(args, "OO", &input1, &input2))
        return NULL;

    if (PyUnicode_Check(input1)) {
        const char *str1 = PyUnicode_AsUTF8AndSize(input1, &len1);
        if (!str1) {
            PyErr_SetString(PyExc_ValueError, "Failed to decode the first string.");
            return NULL;
        }
        array1 = (unsigned char *)str1;
    } else if (PyList_Check(input1)) {
        len1 = PyList_Size(input1);
        array1 = (unsigned char *)malloc(len1 * sizeof(unsigned char));
        if (!array1) {
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory!");
            return NULL;
        }
        for (Py_ssize_t i = 0; i < len1; i++) {
            PyObject *item = PyList_GetItem(input1, i);
            if (!PyLong_Check(item)) {
                PyErr_SetString(PyExc_TypeError, "All elements in the first list must be integers.");
                free(array1);
                return NULL;
            }
            long value = PyLong_AsLong(item);
            if (value < 0 || value > 254) {
                PyErr_SetString(PyExc_ValueError, "All integers must be between 0 and 254.");
                free(array1);
                return NULL;
            }
            array1[i] = (unsigned char)value;
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "The first argument must be a string or a list of integers.");
        return NULL;
    }

    if (PyUnicode_Check(input2)) {
        const char *str2 = PyUnicode_AsUTF8AndSize(input2, &len2);
        if (!str2) {
            PyErr_SetString(PyExc_ValueError, "Failed to decode the second string.");
            if (array1 != (unsigned char *)PyUnicode_AsUTF8AndSize(input1, NULL)) free(array1);
            return NULL;
        }
        array2 = (unsigned char *)str2;
    } else if (PyList_Check(input2)) {
        len2 = PyList_Size(input2);
        array2 = (unsigned char *)malloc(len2 * sizeof(unsigned char));
        if (!array2) {
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory!");
            if (array1 != (unsigned char *)PyUnicode_AsUTF8AndSize(input1, NULL)) free(array1);
            return NULL;
        }
        for (Py_ssize_t i = 0; i < len2; i++) {
            PyObject *item = PyList_GetItem(input2, i);
            if (!PyLong_Check(item)) {
                PyErr_SetString(PyExc_TypeError, "All elements in the second list must be integers.");
                free(array1);
                free(array2);
                return NULL;
            }
            long value = PyLong_AsLong(item);
            if (value < 0 || value > 254) {
                PyErr_SetString(PyExc_ValueError, "All integers must be between 0 and 254.");
                free(array1);
                free(array2);
                return NULL;
            }
            array2[i] = (unsigned char)value;
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "The second argument must be a string or a list of integers.");
        if (array1 != (unsigned char *)PyUnicode_AsUTF8AndSize(input1, NULL)) free(array1);
        return NULL;
    }

    result = lev_dist_single(array1, array2, (size_t)len1, (size_t)len2);

    if (array1 != (unsigned char *)PyUnicode_AsUTF8AndSize(input1, NULL)) free(array1);
    if (array2 != (unsigned char *)PyUnicode_AsUTF8AndSize(input2, NULL)) free(array2);

    if (result == MEM_ERROR) {
        PyErr_SetString(lev_dis_error, "Failed to allocate memory during calculation.");
        return NULL;
    }
    if (PyErr_Occurred()) {
        PyErr_Clear();
    }

    return PyLong_FromLong((int)result);
}

static PyMethodDef levenshteinMethods[] = {
    {"single_dist",  lev_dist_single_py, METH_VARARGS,
     "Calculate levenshtein distance of 2 single array"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef levenshteinmodule = {
    PyModuleDef_HEAD_INIT,
    "levenshtein",
    "Implementation of different methods to calculate Levenshtein distance.",
    -1,
    levenshteinMethods
};

PyMODINIT_FUNC PyInit_levenshtein(void)
{
    PyObject *m;

    m = PyModule_Create(&levenshteinmodule);
    if (m == NULL)
        return NULL;

    lev_dis_error = PyErr_NewException("levenshtein.error", NULL, NULL);
    if (PyModule_AddObjectRef(m, "error", lev_dis_error) < 0) {
        Py_CLEAR(lev_dis_error);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}

int main(int argc, char *argv[])
{
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    if (PyImport_AppendInittab("levenshtein", PyInit_levenshtein) == -1) {
        fprintf(stderr, "Error: could not extend in-built modules table\n");
        exit(1);
    }

    status = PyConfig_SetBytesString(&config, &config.program_name, argv[0]);
    if (PyStatus_Exception(status)) {
        goto exception;
    }

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        goto exception;
    }
    PyConfig_Clear(&config);

    PyObject *pmodule = PyImport_ImportModule("levenshtein");
    if (!pmodule) {
        PyErr_Print();
        fprintf(stderr, "Error: could not import module 'levenshtein'\n");
    }

    return 0;

  exception:
     PyConfig_Clear(&config);
     Py_ExitStatusException(status);
}
