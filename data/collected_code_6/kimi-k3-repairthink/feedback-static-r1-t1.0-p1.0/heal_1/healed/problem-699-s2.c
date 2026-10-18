#include <stdio.h>
#include <string.h>

#define MAX_LEN 100

int min_swaps(const char *str1, const char *str2, size_t max_len) {
    size_t len1 = strnlen(str1, max_len);
    size_t len2 = strnlen(str2, max_len);

    if (len1 == max_len || len2 == max_len) {
        return -1;
    }

    if (len1 != len2) {
        return -1;
    }

    int count0_str1 = 0, count1_str1 = 0;
    int count0_str2 = 0, count1_str2 = 0;

    for (size_t i = 0; i < len1; i++) {
        if (str1[i] == '0') count0_str1++;
        else if (str1[i] == '1') count1_str1++;
        else return -1;

        if (str2[i] == '0') count0_str2++;
        else if (str2[i] == '1') count1_str2++;
        else return -1;
    }

    if (count0_str1 != count0_str2 || count1_str1 != count1_str2) {
        return -1;
    }

    int swaps = 0;
    for (size_t i = 0; i < len1; i++) {
        if (str1[i] != str2[i]) {
            swaps++;
        }
    }

    return swaps / 2;
}

int main(void) {
    char str1[MAX_LEN];
    char str2[MAX_LEN];

    printf("Enter the first binary string: ");
    if (fgets(str1, sizeof(str1), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    str1[strcspn(str1, "\n")] = '\0';

    printf("Enter the second binary string: ");
    if (fgets(str2, sizeof(str2), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    str2[strcspn(str2, "\n")] = '\0';

    int result = min_swaps(str1, str2, sizeof(str1));

    if (result == -1) {
        printf("Conversion not possible.\n");
    } else {
        printf("Minimum number of swaps required: %d\n", result);
    }

    return 0;
}