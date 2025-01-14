from setuptools import setup, Extension

module = Extension(
    "levenshtein",
    sources=["levenshtein_dist.c"],
)

setup(
    name="levenshtein",
    version="1.0",
    description="A module to calculate Levenshtein distance",
    ext_modules=[module],
)
