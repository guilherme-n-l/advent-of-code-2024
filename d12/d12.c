#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LEN ((int)1.96e4)
#define LINE_LEN 140
#define N_CHARS 26

#define UP(A) (A - LINE_LEN < 0 ? -1 : A - LINE_LEN)
#define DOWN(A) (A + LINE_LEN >= INPUT_LEN ? -1 : A + LINE_LEN)
#define LEFT(A) ((A) % LINE_LEN == 0 ? -1 : A - 1)
#define RIGHT(A) ((A + 1) % LINE_LEN == 0 ? -1 : A + 1)
#define POLL(A, B) (A[--B])
#define PILE(A, B, C) (A[B++] = C)
#define ADJACENT(A, B, C) (A[UP(B)] == C || A[DOWN(B)] == C || A[LEFT(B)] == C || A[RIGHT(B)] == C ||)
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))


char input[INPUT_LEN];

void get_input() {
    FILE *f = fopen("input.txt", "r");
    int cursor = 0;
    char ln[LINE_LEN + 2];
    while (fgets(ln, sizeof(ln), f)) {
        memcpy(input + cursor, ln, LINE_LEN);
        cursor += LINE_LEN;
    }
    fclose(f);
}

void get_block(int idx, char visited[], char block[], int* block_end) {
    int stack_len = 0, stack[INPUT_LEN];
    *block_end = INT_MIN;
    PILE(stack, stack_len, idx);

    while (stack_len > 0) {
        int i = POLL(stack, stack_len);
        visited[i] = 1, block[i] = 1;
        *block_end = MAX(*block_end, i);

        int neighs_len = 4, neighs[] = {UP(i), DOWN(i), LEFT(i), RIGHT(i)};
        
        for (int j = 0; j < neighs_len; j++)
            if (neighs[j] != -1 && !visited[neighs[j]] && input[neighs[j]] == input[i])
                PILE(stack, stack_len, neighs[j]);
    }
}

int get_area(int idx, char block[], int block_end) {
    int area = 0;
    for (int i = idx; i < (block_end + 1); i++) if (block[i]) area++;
    return area;
}

int get_perimeter(int idx, char block[], int block_end) {
    int perimeter = 0;

    for (int i = idx; i < (block_end + 1); i++) {
        if (!block[i]) continue;

        int neighs_len = 4, neighs[] = {UP(i), DOWN(i), LEFT(i), RIGHT(i)};

        for (int j = 0; j < neighs_len; j++)
            if (neighs[j] == -1 || !block[neighs[j]])
                perimeter++;
    }
    return perimeter;
}

int get_n_sides(int idx, char block[], int block_end) {
    int n_sides = 0;
    char edges[INPUT_LEN][4] = {0};

    for (int i = idx; i < (block_end + 1); i++) {
        if (!block[i]) continue;

        int neighs_len = 4, neighs[] = {UP(i), DOWN(i), LEFT(i), RIGHT(i)};

        char no_edges = 1;
        for (int j = 0; j < neighs_len; j++)
            if (neighs[j] == -1 || !block[neighs[j]]) {
                no_edges = 0;
                break;
            }

        if (no_edges) continue;

        char *neigh_edges[4] = {NULL, NULL, NULL, NULL};
        for (int j = 0; j < neighs_len; j++)
            if (neighs[j] != -1 && block[neighs[j]]) 
                neigh_edges[j] = edges[neighs[j]];

        for (int j = 0; j < neighs_len; j++) {
            if (neighs[j] != -1 && block[neighs[j]]) continue;
            edges[i][j] = 1;

            int neigh_search_len = 2, neigh_search[] = {
                j > 1 ? 0 : 2,
                j > 1 ? 1 : 3,
            };

            char new_side = 1;
            for (int k = 0; k < neigh_search_len; k++) 
                if (neigh_edges[neigh_search[k]] && neigh_edges[neigh_search[k]][j]) {
                    new_side = 0;
                    break;
                }

            if (new_side) n_sides++;
        }
    }

    return n_sides;
}

int main() {
    get_input();

    char visited[INPUT_LEN] = {0};
    long collector = 0, collector2 = 0;

    for (int i = 0; i < INPUT_LEN; i++) {
        if (visited[i]) continue;

        int block_end;
        char block[INPUT_LEN] = {0};
        get_block(i, visited, block, &block_end);

        int area = get_area(i, block, block_end);
        collector += area * get_perimeter(i, block, block_end);
        collector2 += area * get_n_sides(i, block, block_end);
        
    }

    printf("%ld\n", collector);
    printf("%ld\n", collector2);
    return 0;
}
