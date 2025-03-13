#include <stdlib.h>

#include "mis.h"


static PyObject* max_independent_set(PyObject *self, PyObject *args) {
    PyObject *py_list;
    const int iterations;
    const double init_temp;
    const double cooling_rate;

    if (!PyArg_ParseTuple(args, "Oidd", &py_list, &iterations, &init_temp, &cooling_rate)) {
        return NULL;  // Python error is set automatically
    }


    int n;
    int **adj = parse_python_list(py_list, &n);

    if (adj == NULL) {
        return PyErr_NoMemory();
    }


    bool *independent_set = calloc(n, sizeof(bool));

    simulated_annealing_mis(adj, n, independent_set, iterations, init_temp, cooling_rate);

    // Convert result to a Python list
    PyObject *result = PyList_New(0);
    for (int i = 0; i < n; i++) {
        if (independent_set[i]) {
            PyList_Append(result, PyLong_FromLong(i));
        }
    }

    free(independent_set);
    free_2d_array(adj, n);
    return result;
}

static PyMethodDef MISMethods[] = {
    {"max_independent_set", max_independent_set, METH_VARARGS, "Find a Maximum Independent Set"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef mis_module = {
    PyModuleDef_HEAD_INIT, "_mis_finder_core", NULL, -1, MISMethods
};

PyMODINIT_FUNC PyInit__mis_finder_core() {
    return PyModule_Create(&mis_module);
}
