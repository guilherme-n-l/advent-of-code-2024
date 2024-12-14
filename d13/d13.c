#include <gsl/gsl_matrix_double.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector_double.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_linalg.h>
#include <math.h>

#define LINE_LEN 23
#define N_MACHINES 320
#define SEP "+= ,\n"

typedef struct {
    int a_x;
    int a_y;
    int b_x;
    int b_y;
    int p_x;
    int p_y;
} claw_t;

int input_len = 0;
claw_t input[N_MACHINES];

char a_b_solution(claw_t c, int solution[]) {
    gsl_matrix *coefs = gsl_matrix_alloc(2, 2);
    gsl_matrix_set(coefs, 0, 0, c.a_x);
    gsl_matrix_set(coefs, 0, 1, c.b_x);
    gsl_matrix_set(coefs, 1, 0, c.a_y);
    gsl_matrix_set(coefs, 1, 1, c.b_y);

    gsl_vector *vars = gsl_vector_alloc(2), *sol = gsl_vector_alloc(2);

    gsl_vector_set(sol, 0, c.p_x);
    gsl_vector_set(sol, 1, c.p_y);

    int perm_sig;
    gsl_permutation *perm = gsl_permutation_alloc(2);

    if(gsl_linalg_LU_decomp(coefs, perm, &perm_sig) != 0) {
        gsl_matrix_free(coefs);
        gsl_vector_free(vars);
        gsl_vector_free(sol);
        gsl_permutation_free(perm);
        return 0;
    }

    gsl_linalg_LU_solve(coefs, perm, sol, vars);

    double sol_a = gsl_vector_get(vars, 0), sol_b = gsl_vector_get(vars, 1);
    int a = (int)round(sol_a), b = (int)round(sol_b);

    gsl_matrix_free(coefs);
    gsl_vector_free(vars);
    gsl_vector_free(sol);
    gsl_permutation_free(perm);

    if (fabs(sol_a - a) < 1e-6 && fabs(sol_b - b) < 1e-6) {
        solution[0] = a;
        solution[1] = b;
        return 1;
    }

    return 0;
}

void get_input() {
    FILE *f = fopen("input.txt", "r");
    char ln[LINE_LEN + 2];
    int buf_len = 0, buf[6];

    while (fgets(ln, sizeof(ln), f)) {
        char *tok = strtok(ln, SEP);

        while (tok) {
            for (char c = '0'; c <= '9'; c++) {
                if (tok[0] == c) {
                    buf[buf_len++] = atoi(tok);

                    if (buf_len == 6) {
                        input[input_len].a_x = buf[0];
                        input[input_len].a_y = buf[1];
                        input[input_len].b_x = buf[2];
                        input[input_len].b_y = buf[3];
                        input[input_len].p_x = buf[4];
                        input[input_len++].p_y = buf[5];

                        buf_len = 0;
                    }
                }
            }
            tok = strtok(NULL, SEP);
        }
    }
    fclose(f);
}

int main() {
    get_input();

    long collector = 0;
    for (int i = 0; i < input_len; i++) {
        claw_t c = input[i];
        int c_solution[2];

        char has_solution = a_b_solution(c, c_solution);

        if (!has_solution || c_solution[0] > 100 || c_solution[1] > 100) continue;

        collector += 3 * c_solution[0] + c_solution[1];
    }

    printf("%ld\n", collector);
    return 0;
}
