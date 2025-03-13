#include <stdlib.h>

#include "mis.h"


int** parse_python_list(PyObject* py_list, int* size) {
    if (!PyList_Check(py_list)) {
        PyErr_SetString(PyExc_TypeError, "Expected a list of lists.");
        return NULL;
    }

    *size = PyList_Size(py_list);
    int** array = (int**)malloc(*size * sizeof(int*));
    if (!array) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory.");
        return NULL;
    }

    for (int i = 0; i < *size; i++) {
        PyObject* row = PyList_GetItem(py_list, i);  // Borrowed reference
        if (!PyList_Check(row) || PyList_Size(row) != *size) {
            PyErr_SetString(PyExc_TypeError, "Expected a square list of lists.");
            free(array);
            return NULL;
        }

        array[i] = (int*)malloc(*size * sizeof(int));
        if (!array[i]) {
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate row memory.");
            // Free previously allocated rows
            for (int j = 0; j < i; j++) free(array[j]);
            free(array);
            return NULL;
        }

        for (int j = 0; j < *size; j++) {
            PyObject* item = PyList_GetItem(row, j);
            if (!PyLong_Check(item)) {
                PyErr_SetString(PyExc_TypeError, "List elements must be integers.");
                // Cleanup
                for (int k = 0; k <= i; k++) free(array[k]);
                free(array);
                return NULL;
            }
            array[i][j] = (int)PyLong_AsLong(item);
        }
    }

    return array;
}

void free_2d_array(int** array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
        array[i] = NULL;
    }
    free(array);
    array = NULL;
}
