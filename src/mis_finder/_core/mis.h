#ifndef MIS
#define MIS

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdbool.h>

int** parse_python_list(PyObject* , int*);

void free_2d_array(int**, int);

int** create_matrix(int);

void free_matrix(int**, int);

void simulated_annealing_mis(int**, int, bool*, int, double, double);

#endif
