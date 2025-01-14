# levenshtein_distance

**levenshtein_distance** is a Python module for calculating the Levenshtein distance, implemented in C to provide high performance. This module supports computing the distance between strings and arrays of integers.

## Features

- **High Performance**: Core algorithm implemented in C for optimal speed.
- **Versatile Input**: Supports strings and arrays of integers as input.
- **Python Integration**: Exposes a simple Python interface for seamless integration with Python projects.

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/AliSalehian/levenshtein_distance.git
    cd levenshtein_distance
    ```
2. Build and install the module:
    ```bash
    python3 setup.py build
    python3 setup.py install
    ```

## Usage

### Example: Calculating Levenshtein Distance

For Strings:
```python
import levenshtein

str1 = "kitten"
str2 = "sitting"
distance = levenshtein.single_dist(str1, str2)
print(f"Levenshtein Distance: {distance}")
```

For Arrays of Integers:
```python
import levenshtein

arr1 = [32, 39, 53] # حسن
arr2 = [21, 36, 52, 25, 53] # آرمان
distance = levenshtein.single_dist(arr1, arr2)
print(f"Levenshtein Distance: {distance}")
```

## API Reference

### `levenshtein.single_dist(input1, input2)`

- **Description:** Computes the Levenshtein distance between two inputs.
- **Parameters:**
    - `input1` (`str` or `list[int]`): The first input, either a string or a list of integers.
    - `input2` (str or `list[int]`): The second input, either a string or a list of integers.
- **Returns:** The Levenshtein distance (`int`) between the two inputs.
- **Raises:**
    - `levenshtein.error`: If an internal error occurs during computation.
    - `TypeError`: If the inputs are of unsupported types.
    - `ValueError`: If list elements are out of range (0-254).

## Development

### Build the Module Locally

1. Ensure you have Python development headers installed:
    ```bash
    sudo apt-get install python3-dev
    ```
2. Build and install the module:
    ```bash
    python3 setup.py build
    python3 setup.py install --user
    ```
3. Run tests to verify:
    ```bash
    cd test
    python3 -m unittest -v leveneshtein_distance_test.py
    ```

### Contributing

Contributions are welcome! Please follow these steps:
1. Fork the repository.
2. Create a new branch: `git checkout -b feature/your-feature`.
3. Commit your changes: `git commit -am 'Add some feature'`.
4. Push to the branch: `git push origin feature/your-feature`.
5. Open a pull request.
