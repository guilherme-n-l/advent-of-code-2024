#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#define MIN(A, B) ((A) < (B) ? (A) : (B))

int main() {
    FILE *f = fopen("input.txt", "r");
    char ln[3500], raw_str[18865];

    while (fgets(ln, sizeof(ln), f)) {
        ln[strlen(ln) - 1] = '\0';
        strcat(raw_str, ln);
    }
    fclose(f);

    int raw_str_len = strlen(raw_str);
    long collector = 0;

    for (int i = 0; i < raw_str_len; i++) {
        char subs[4 + 1];
        if (i + 4 >= raw_str_len || raw_str[i] != 'm') {
            strncpy(subs, raw_str + i, 4);
            subs[4] = '\0';
            if (strcmp("mul(", subs) != 0) continue;
        }

        i += 4;
        if (i >= raw_str_len) break;
        
        short nums[2] = {SHRT_MIN, SHRT_MIN};
        char terminators[2] = {',', ')'};

        for (int j = 0; j < 2; j++) {
            int start_number = i;

            for (int k = 0; k < 3; k++) {
                if (!isdigit(raw_str[i])) break;
                i++;
            }
            
            if (raw_str[i] != terminators[j]) break;

            char* number = malloc((3 + 1) * sizeof(char));
            strncpy(number, raw_str + start_number, i - start_number);
            number[i - start_number] = '\0';

            nums[j] = atoi(number);
            free(number);
            i++;
        }

        if (nums[0] == SHRT_MIN || nums[1] == SHRT_MIN) continue;
        collector += nums[0] * nums[1];
        i--;
    }
    printf("%ld\n", collector);
    return 0;
}
