#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "mis.h"


struct cost_calc_result {
    int cost;
    bool is_valid;
};


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
    // Calculate res = x_temp x
    for (int k = 0; k < n; k++) {
        res = res + x_temp[k] * x[k];

    }

    free(x_temp);
    x_temp = NULL;

    return res;
}


static struct cost_calc_result cost_function(
    int** adj, int n, bool* independent_set, int **Q
) {
    struct cost_calc_result res;

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
    return res;
}


static void accept_choice(int n, bool* candidate, bool* result) {
    for (int i = 0; i < n; i++) {
        result[i] = candidate[i];
    }
}

static bool should_accept(double current_temp, int cost_current, int cost_candidate) {
    int delta_e = cost_candidate - cost_current;

    if (delta_e < 0) return true;

    double acceptance_prob = exp(-((double)delta_e / current_temp));

    return ((double)rand() / RAND_MAX) < acceptance_prob;
}

static bool is_independent(int n, int** adj, bool *chosen_set) {
    for (int i = 0; i < n; i++) {
        if (chosen_set[i] != 1) continue;

        for (int j = 0; j < n; j++) {
            if (chosen_set[j] != 1) continue;

            if (adj[i][j] != 0) return false;
        }
    }
    return true;
}


static void make_choice(int n, bool* buffer, int** adj) {
    int choice;

    for (int i = 0; i < n; i++) {
        choice = rand() & 1;
        buffer[i] = choice;
        }

}

static void handle_empty_final_result(int n, bool* result) {
    for (int i = 0; i < n; i++) {
        if (result[i] == 1) {  // Result is not empty
            return;
        }
    }
    // All entries are 0 - choose one random index
    int rand_idx = rand() % n;
    result[rand_idx] = 1;
}


void simulated_annealing_mis(
    int** adj, int n, bool* independent_set, int its, double init_t, double c_rate
) {
    srand(time(NULL));

    int **Q = create_matrix(n);

    double current_temp = init_t;

    struct cost_calc_result cost_res_current, cost_res_candidate;

    bool *candidate = calloc(n, sizeof(bool));

    // initial solution
    int rand_idx = rand() % n;
    candidate[rand_idx] = 1;

    cost_res_current = cost_function(adj, n, independent_set, Q);

    for (int i = 0; i < its; i++) {
        make_choice(n, candidate, adj);
        if (!is_independent(n, adj, candidate)) {
            continue;
        }

        cost_res_candidate = cost_function(adj, n, candidate, Q);

        if (should_accept(current_temp, cost_res_current.cost, cost_res_candidate.cost)) {
            accept_choice(n, candidate, independent_set);
            cost_res_current.cost = cost_res_candidate.cost;
        }

        current_temp = current_temp * c_rate;
    }

    handle_empty_final_result(n, independent_set);

    free(candidate);
    free_matrix(Q, n);
    candidate = NULL;
}
