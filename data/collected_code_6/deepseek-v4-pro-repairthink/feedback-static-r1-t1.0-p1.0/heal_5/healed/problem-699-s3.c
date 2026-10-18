#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min_swaps(const char *s1, const char *s2) {
    size_t i = 0;
    int count1_0 = 0, count1_1 = 0;
    int count2_0 = 0, count2_1 = 0;
    int mismatch_0_to_1 = 0, mismatch_1_to_0 = 0;

    while (1) {
        char c1 = s1[i];
        char c2 = s2[i];

        if (c1 == '\0' && c2 == '\0')
            break;
        if (c1 == '\0' || c2 == '\0')
            return -1;

        if (c1 != '0' && c1 != '1')
            return -1;
        if (c2 != '0' && c2 != '1')
            return -1;

        if (c1 == '0')
            count1_0++;
        else
            count1_1++;
        if (c2 == '0')
            count2_0++;
        else
            count2_1++;

        if (c1 != c2) {
            if (c1 == '0')
                mismatch_0_to_1++;
            else
                mismatch_1_to_0++;
        }
        i++;
    }

    if (count1_0 != count2_0 || count1_1 != count2_1)
        return -1;
    if (mismatch_0_to_1 != mismatch_1_to_0)
        return -1;

    return mismatch_0_to_1;
}

char *read_line(void) {
    size_t size = 128;
    size_t len = 0;
    char *buf = malloc(size);
    if (!buf) return NULL;
    buf[0] = '\0';

    while (fgets(buf + len, size - len, stdin)) {
        len += strlen(buf + len);
        if (buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
            return buf;
        }
        if (size - len <= 1) {
            size_t new_size = size * 2;
            char *new_buf = realloc(buf, new_size);
            if (!new_buf) {
                free(buf);
                return NULL;
            }
            buf = new_buf;
            size = new_size;
        }
    }

    if (len == 0) {
        free(buf);
        return NULL;
    }
    buf[len] = '\0';
    return buf;
}

int main(void) {
    char *str1, *str2;

    printf("Enter first binary string: ");
    str1 = read_line();
    if (!str1) {
        printf("Error reading input\n");
        return 1;
    }

    printf("Enter second binary string: ");
    str2 = read_line();
    if (!str2) {
        printf("Error reading input\n");
        free(str1);
        return 1;
    }

    int result = min_swaps(str1, str2);

    if (result == -1) {
        printf("Conversion not possible\n");
    } else {
        printf("Minimum swaps required: %d\n", result);
    }

    free(str1);
    free(str2);
    return 0;
}