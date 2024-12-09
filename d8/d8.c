#include <stdio.h>
#include <string.h>

#define CHARS_LEN 36
#define MAX_CHAR_COUNT 4
#define LINE_LEN 50
#define INPUT_LEN ((int)2500)
#define _X(A) (A / LINE_LEN)
#define _Y(A) (A - _X(A) * LINE_LEN)

int main() {
    FILE *f = fopen("input.txt", "r");

    char ln[LINE_LEN + 2], input[INPUT_LEN] = "\0";
    while (fgets(ln, sizeof(ln), f)) strncat(input, ln, LINE_LEN);

    char chars[CHARS_LEN + 1] = "\0", char_idx_arr_len[CHARS_LEN] = {0};
    int char_idx_arr[CHARS_LEN][MAX_CHAR_COUNT];

    for (int i = 0; i < INPUT_LEN; i++) {
        if (input[i] == '\n' || input[i] == '.') continue;

        char *pos = strchr(chars, input[i]);
        if (!pos) strcat(chars, (char[]){input[i], '\0'});
        int idx = pos ? (pos - chars) : ((int)strlen(chars) - 1);

        char_idx_arr[idx][char_idx_arr_len[idx]++] = i;
    }

    char has_interference[INPUT_LEN] = {0};
    int collector = 0;
    for (int i = 0; i < strlen(chars); i++) {
        for (int j = 0; j < char_idx_arr_len[i]; j++) {
            int jdx = char_idx_arr[i][j], jdx_x = _X(jdx), jdx_y = _Y(jdx);
            for (int k = 0; k < char_idx_arr_len[i]; k++) {
                if (j == k) continue;

                int kdx = char_idx_arr[i][k], kdx_x = _X(kdx), kdx_y = _Y(kdx),
                interf_x = jdx_x - (kdx_x - jdx_x), interf_y = jdx_y - (kdx_y - jdx_y);

                if (
                    interf_x < 0 ||
                    interf_x >= LINE_LEN ||
                    interf_y < 0 ||
                    interf_y >= LINE_LEN
                ) continue;

                int interf_idx = (interf_x * LINE_LEN) + interf_y;

                if (!has_interference[interf_idx]) {
                    has_interference[interf_idx]++;
                    collector++;
                }
            }
        }
    }

    printf("%d\n", collector);

    memset(has_interference, 0, sizeof(has_interference));
    collector = 0;
    for (int i = 0; i < strlen(chars); i++) {
        for (int j = 0; j < char_idx_arr_len[i]; j++) {
            int jdx = char_idx_arr[i][j], jdx_x = _X(jdx), jdx_y = _Y(jdx);
            for (int k = 0; k < char_idx_arr_len[i]; k++) {
                if (j == k) continue;

                int kdx = char_idx_arr[i][k], kdx_x = _X(kdx), kdx_y = _Y(kdx),
                interf_x = jdx_x, interf_y = jdx_y, interf_idx = (interf_x * LINE_LEN) + interf_y;;

                if (!has_interference[interf_idx]) {
                    has_interference[interf_idx]++;
                    collector++;
                }

                while (1) {
                    interf_x -= kdx_x - jdx_x, interf_y -= kdx_y - jdx_y;
                    if (
                            interf_x < 0 ||
                            interf_x >= LINE_LEN ||
                            interf_y < 0 ||
                            interf_y >= LINE_LEN
                       ) break;

                    interf_idx = (interf_x * LINE_LEN) + interf_y;

                    if (!has_interference[interf_idx]) {
                        has_interference[interf_idx]++;
                        collector++;
                    }
                }
            }
        }
    }

    printf("%d\n", collector);
    return 0;
}
