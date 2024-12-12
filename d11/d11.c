#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib-2.0/glib.h>

#define LINE_LEN 36
#define REPEAT 25
#define REPEAT_PT2 75
#define NUMS_LEN 8
#define POLL(A, B) (A[--B])
#define PILE(A, B, C) (A[B++] = C)

int nums[NUMS_LEN], nums_len = 0;
GHashTable *blink_cache, *recursive_cache; 

typedef struct {long l1; long l2;} longs;

void get_input() {
    FILE *f = fopen("input.txt", "r");
    char ln[LINE_LEN + 2];
    fgets(ln, sizeof(ln), f);
    
    char *tok = strtok(ln, " ");

    while (tok) {
        nums[nums_len++] = atoi(tok);
        tok = strtok(NULL, " ");
    }
}

longs blink(long n) {
    char num_str[250];

    longs *val_ptr = g_hash_table_lookup(blink_cache, GUINT_TO_POINTER(n));

    if (val_ptr) return *val_ptr;

    longs *val = g_malloc(sizeof(longs));
    switch (n) {
        case 0: {
            val->l1 = 1, val->l2 = -1;
            g_hash_table_insert(blink_cache, GUINT_TO_POINTER(n), val);
            return *val;
        }
        default:
            sprintf(num_str, "%ld", n); 

            if (strlen(num_str) & 1) {
                val->l1 = 2024 * n, val->l2 = -1;
                g_hash_table_insert(blink_cache, GUINT_TO_POINTER(n), val);
                return *val;
            }
            else {
                char slice[125];

                int half_size = strlen(num_str) / 2;

                strncpy(slice, num_str, half_size);
                slice[half_size] = '\0';
                val->l1 = atol(slice);

                strncpy(slice, num_str + half_size, half_size);
                slice[half_size] = '\0';
                val->l2 = atol(slice);

                g_hash_table_insert(blink_cache, GUINT_TO_POINTER(n), val);
                return *val;
            }
    }
}

long recursive(long id, long d, int repeat) {
    if (d < repeat) {
        longs blinked = blink(id);

        longs *left = g_malloc(sizeof(longs));
        left->l1 = blinked.l1, left->l2 = d + 1;
        long *left_val_ptr = g_hash_table_lookup(recursive_cache, left);
        
        long l_res;
        if (!left_val_ptr) {
            l_res = recursive(blinked.l1, d + 1, repeat);
            left_val_ptr = g_malloc(sizeof(long));
            *left_val_ptr = l_res;
            g_hash_table_insert(recursive_cache, left, left_val_ptr);
        } else {
            g_free(left);
            l_res = *left_val_ptr;
        }

        if (blinked.l2 == -1) return l_res;

        longs *right = g_malloc(sizeof(longs));
        right->l1 = blinked.l2, right->l2 = d + 1;
        long *right_val_ptr = g_hash_table_lookup(recursive_cache, right);
        
        long r_res;
        if (!right_val_ptr) {
            r_res = recursive(blinked.l2, d + 1, repeat);
            right_val_ptr = g_malloc(sizeof(long));
            *right_val_ptr = r_res;
            g_hash_table_insert(recursive_cache, right, right_val_ptr);
        } else {
            g_free(right);
            r_res = *right_val_ptr;
        }

        return l_res + r_res;
    }

    return 1;
}

gboolean equal(gconstpointer a, gconstpointer b) {
    const longs *a_ptr = (longs *)a;
    const longs *b_ptr = (longs *)b;
    return a_ptr->l1 == b_ptr->l1 && a_ptr->l2 == b_ptr->l2;
}

guint hash(gconstpointer a) {
    const longs *a_ptr = (longs *)a;
    return g_int_hash(&a_ptr->l1) ^ g_int_hash(&a_ptr->l2);
}

void free_values(gpointer k, gpointer v, gpointer args) {
    g_free(v);
}

void free_keys(gpointer k, gpointer v, gpointer args) {
    g_free(k);
}

long solve(int repeat) {
    blink_cache = g_hash_table_new(g_direct_hash, g_direct_equal);
    recursive_cache = g_hash_table_new(hash, equal);

    long collector = 0;
    for (int i = 0; i < nums_len; i++) collector += recursive(nums[i], 0, repeat);

    g_hash_table_foreach(blink_cache, free_values, NULL);
    g_hash_table_destroy(blink_cache);
    g_hash_table_foreach(recursive_cache, free_keys, NULL);
    g_hash_table_foreach(recursive_cache, free_values, NULL);
    g_hash_table_destroy(recursive_cache);
    return collector;
}

int main() {
    get_input();
    printf("%ld\n", solve(REPEAT));
    printf("%ld\n", solve(REPEAT_PT2));

    return 0;
}
