#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LEN (int)1e3

int main() {
    FILE *f = fopen("input.txt", "r");
    char buf[256];

    int collector = 0, collector2 = 0;

    while (fgets(buf, sizeof(buf), f)) {
        int *arr = malloc(64 * sizeof(int)), arr_len = 0;

        char *tok = strtok(buf, " ");
        while (tok) {
            arr[arr_len++] = atoi(tok);
            tok = strtok(NULL, " ");
        }

        char increases = arr[0] < arr[1], finished = 1;
        for (int i = 0; i < arr_len - 1; i++) {
            if (
                (arr[i] < arr[i + 1]) != increases ||
                arr[i] == arr[i + 1] ||
                abs(arr[i + 1] - arr[i]) > 3
            ) {
                finished = 0;
                break;
            }
        }

        if (finished) {
            collector++;
            continue;
        }

        for (int i = 0; i < arr_len; i++) {
            int n_arr_len = arr_len - 1, *n_arr = malloc(n_arr_len * sizeof(int));


            memcpy(n_arr, arr, i * sizeof(int));
            memcpy(n_arr + i, arr + i + 1, (n_arr_len - i) * sizeof(int));

            increases = n_arr[0] < n_arr[1], finished = 1;
            for (int j = 0; j < n_arr_len - 1; j++) {
                if (
                        (n_arr[j] < n_arr[j + 1]) != increases ||
                        n_arr[j] == n_arr[j + 1] ||
                        abs(n_arr[j + 1] - n_arr[j]) > 3
                   ) {
                    finished = 0;
                    break;
                }
            }

            free(n_arr);

            if (finished) {
                collector2++;
                break;
            }
        }
    }
    printf("%d\n", collector);
    printf("%d\n", collector + collector2);


    fclose(f);

    return 0;
}
