#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 36
#define REPEAT 25
#define NUMS_LEN 8

int main() {
    FILE *f = fopen("input.txt", "r");
    char ln[LINE_LEN + 2];
    fgets(ln, sizeof(ln), f);
    
    char *tok = strtok(ln, " ");
    int nums[NUMS_LEN], nums_len = 0;

    while (tok) {
        nums[nums_len++] = atoi(tok);
        tok = strtok(NULL, " ");
    }

    int tmp_len = nums_len;
    long *tmp = malloc(nums_len * sizeof(long));
    for (int i = 0; i < nums_len; i++) tmp[i] = (long)nums[i];

    for (int i = 0; i < REPEAT; i++) {
        int blinked_len = 2 * tmp_len;
        long *blinked = malloc(blinked_len * sizeof(long));

        blinked_len = 0;

        for (int j = 0; j < tmp_len; j++) {

            char num_str[250];
            switch (tmp[j]) {
                case 0:
                    blinked[blinked_len++] = 1;
                    break;

                default:
                    sprintf(num_str, "%ld", tmp[j]); 
                    if (strlen(num_str) & 1) blinked[blinked_len++] = tmp[j] * 2024;
                    else {
                        char slice[125];
                        for (int k = 0; k < 2; k++) {
                            int half_size = strlen(num_str) / 2;
                            strncpy(slice, num_str + k * half_size, half_size);
                            slice[half_size] = '\0';
                            blinked[blinked_len++] = atol(slice);
                        }
                    }
            }
        }
        free(tmp);
        tmp = blinked, tmp_len = blinked_len;
    }

    printf("%d\n", tmp_len);
    free(tmp);

    return 0;
}
