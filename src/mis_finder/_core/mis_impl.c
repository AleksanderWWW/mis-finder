#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define INIT_TEMP 1000.0   // Initial temperature
#define COOLING_RATE 0.995 // Cooling rate per iteration
#define ITERATIONS 1000  // Max iterations per thread


struct cost_calc_result {
    int cost;
    bool is_valid;
};


static int** create_matrix(int n) {
    int **matrix = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(int));
    }

    return matrix;
}


static void free_matrix(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
        matrix[i] = NULL;
    }
    free(matrix);
    matrix = NULL;
}


static int qubo(int** Q, bool* x, int n) {
    int temp_res;
    int res = 0;
    int* x_temp = calloc(n, sizeof(int));

    // Calculate x_temp = x^T Q
    for (int i = 0; i < n; i++) {
        temp_res = 0;
        for (int j = 0; j < n; j++) {
            temp_res = temp_res + x[i] * Q[j][i];
        }
        x_temp[i] = temp_res;
    }
//    // Calculate res = x_temp x
    for (int k = 0; k < n; k++) {
        res = res + x_temp[k] * x[k];

    }

    free(x_temp);
    x_temp = NULL;

    return res;
}


static struct cost_calc_result cost_function(int** adj, int n, bool* independent_set) {
    struct cost_calc_result res;

    int **Q = create_matrix(n);

    int i;
    i = 0;

    for (; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                Q[i][j] = -1;
            }
            else if (adj[i][j]) {
                Q[i][j] = 2;
            }
            else {
                Q[i][j] = 0;
            }

        }
    }
    res.cost = qubo(Q, independent_set, n);
    res.is_valid = true;
    free_matrix(Q, n);
    return res;
}


void simulated_annealing_mis(int** adj, int n, bool* independent_set) {
    srand(time(NULL));

    int choice;
    struct cost_calc_result cost_res;

    // initial random guess
    for (int i = 0; i < n; i++) {
        choice = rand() & 1;
        independent_set[i] = choice;
    }

    cost_res = cost_function(adj, n, independent_set);

    printf("%d\n", cost_res.cost);
}
