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
GHashTable *ht; 

typedef struct {long l1; long l2;} longs;

longs blink(long n) {
    char num_str[250];

    longs *val_ptr = g_hash_table_lookup(ht, GUINT_TO_POINTER(n));

    if (val_ptr) return *val_ptr;

    longs *val = g_malloc(sizeof(longs));
    switch (n) {
        case 0: {
            val->l1 = 1, val->l2 = -1;
            g_hash_table_insert(ht, GUINT_TO_POINTER(n), val);
            return *val;
        }
        default:
            sprintf(num_str, "%ld", n); 

            if (strlen(num_str) & 1) {
                val->l1 = 2024 * n, val->l2 = -1;
                g_hash_table_insert(ht, GUINT_TO_POINTER(n), val);
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

                g_hash_table_insert(ht, GUINT_TO_POINTER(n), val);
                return *val;
            }
    }
}

long solve(int repeat) {
    int num_depth_len  = 0;
    longs num_depth[nums_len + REPEAT_PT2];
    long collector = 0;

    for (int i = 0; i < nums_len; i++) PILE(num_depth, num_depth_len, ((longs){nums[i], 0}));

    while (num_depth_len > 0) {
        longs head = POLL(num_depth, num_depth_len);

        if (head.l2 == repeat) {
            collector++;
            continue;
        }
        
        longs blinked = blink(head.l1);
        PILE(num_depth, num_depth_len, ((longs){blinked.l1, head.l2 + 1}));
        if (blinked.l2 != -1) PILE(num_depth, num_depth_len, ((longs){blinked.l2, head.l2 + 1}));
    }

    return collector;
}

void print(gpointer k, gpointer v, gpointer args) {
    long key = GPOINTER_TO_UINT(k);
    longs *val = (longs *)v;

    printf("%ld -> %ld %ld\n", key, val->l1, val->l2);
}

void free_longs(gpointer k, gpointer v, gpointer args) {
    g_free(v);
}

int main() {
    FILE *f = fopen("input.txt", "r");
    char ln[LINE_LEN + 2];
    fgets(ln, sizeof(ln), f);
    
    char *tok = strtok(ln, " ");

    while (tok) {
        nums[nums_len++] = atoi(tok);
        tok = strtok(NULL, " ");
    }

    ht = g_hash_table_new(g_direct_hash, g_direct_equal);
    printf("%ld\n", solve(REPEAT));
    printf("%ld\n", solve(REPEAT_PT2));
    g_hash_table_foreach(ht, free_longs, NULL);
    g_hash_table_destroy(ht);

    return 0;
}
