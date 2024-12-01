#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define INPUT_LEN (int)1e3
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define UP(i) ((i - 1) >> 1)
#define LEFT(i) (2 * i + 1)
#define RIGHT(i) (LEFT(i) + 1)

typedef struct {
    size_t      len;
    size_t      size;
    int         *arr;
} pq_t;

void sift_down(pq_t *pq, size_t i, char is_left) {
    size_t child_idx = is_left ? LEFT(i) : RIGHT(i);

    int tmp = pq->arr[i];
    pq->arr[i] = pq->arr[child_idx];

    pq->arr[child_idx] = tmp;
}

void sift_up(pq_t *pq, size_t i) {
    int tmp = pq->arr[i];
    pq->arr[i] = pq->arr[UP(i)];

    pq->arr[UP(i)] = tmp;
}

pq_t *pq_create(size_t size) {
    pq_t *pq_ptr = malloc(sizeof(pq_t));
    pq_ptr->arr = malloc(size * sizeof(int));
    pq_ptr->size = size;
    pq_ptr->len = 0;

    return pq_ptr;
}

void pq_destroy(pq_t *pq) {
    free(pq->arr);
    free(pq);
}

void pq_insert(pq_t *pq, int i) {
    if (pq->len + 1 > pq->size) abort();

    size_t idx = pq->len;
    pq->arr[pq->len++] = i;
    
    while (idx > 0 && pq->arr[UP(idx)] > i) {
        sift_up(pq, idx);
        idx = UP(idx);
    }
}

int *pq_peek(pq_t *pq) {
    return &pq->arr[0];
}

int pq_remove(pq_t *pq) {
    if (pq->len == 0) abort();

    int return_i = *pq_peek(pq);

    size_t idx = --pq->len;

    if (idx == 0) return return_i;

    pq->arr[0] = pq->arr[idx];

    idx = 0;
    while(LEFT(idx) < pq->len) {
        int l = pq->arr[LEFT(idx)], r = RIGHT(idx) < pq->len ? pq->arr[RIGHT(idx)] : INT_MAX;
        int min_val = MIN(l, r);

        if (min_val >= pq->arr[idx]) break;

        char is_left = min_val == l;
        sift_down(pq, idx, is_left);
        idx = is_left ? LEFT(idx) : RIGHT(idx);
    }
    return return_i;
}


int main() {
    FILE *f = fopen("input.txt", "r");
    int buf[2], *second = malloc(INPUT_LEN * sizeof(int)), second_len = 0, max = INT_MIN;

    pq_t *pq_a = pq_create(INPUT_LEN), *pq_b = pq_create(INPUT_LEN);

    while (fscanf(f, "%d %d", &buf[0], &buf[1]) == 2) max = MAX(max, buf[0]);

    char *present_vals = calloc(max, sizeof(char));

    rewind(f);
    while (fscanf(f, "%d %d", &buf[0], &buf[1]) == 2) {
        pq_insert(pq_a, buf[0]);
        pq_insert(pq_b, buf[1]);

        second[second_len++] = buf[1];
        present_vals[buf[0]] = 1;
    }

    int collector = 0;
    for (int i = 0; i < INPUT_LEN; i++) collector += abs(pq_remove(pq_b) - pq_remove(pq_a));

    printf("%d\n", collector);
    pq_destroy(pq_a);
    pq_destroy(pq_b);

    collector = 0;
    for (int i = 0; i < INPUT_LEN; i++) {
        if (second[i] >= max) continue;
        if (!present_vals[second[i]]) continue;

        collector += second[i];
    }

    free(second);

    printf("%d\n", collector);

    return 0;
}
