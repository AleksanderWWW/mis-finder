#ifndef MIS
#define MIS

int** parse_numpy_matrix(PyArrayObject*, int*);

void simulated_annealing_mis(int**, int, bool*);

#endif
