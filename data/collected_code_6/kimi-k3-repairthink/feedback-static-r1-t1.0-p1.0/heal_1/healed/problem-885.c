#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 256

bool areIsomorphic(const char *str1, const char *str2, size_t max_len)
{
    if (str1 == NULL || str2 == NULL || max_len == 0) {
        return false;
    }

    size_t len1 = strnlen(str1, max_len);
    size_t len2 = strnlen(str2, max_len);

    if (len1 != len2) {
        return false;
    }

    int map1[256] = {0};
    int map2[256] = {0};

    for (size_t i = 0; i < len1; i++) {
        unsigned char c1 = (unsigned char)str1[i];
        unsigned char c2 = (unsigned char)str2[i];

        if (map1[c1] == 0 && map2[c2] == 0) {
            map1[c1] = c2 + 1;
            map2[c2] = c1 + 1;
        } else if (map1[c1] != c2 + 1 || map2[c2] != c1 + 1) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    char str1[BUFFER_SIZE] = {0};
    char str2[BUFFER_SIZE] = {0};

    printf("Enter first string: ");
    if (fgets(str1, sizeof(str1), stdin) == NULL) {
        return 1;
    }
    str1[sizeof(str1) - 1] = '\0';

    printf("Enter second string: ");
    if (fgets(str2, sizeof(str2), stdin) == NULL) {
        return 1;
    }
    str2[sizeof(str2) - 1] = '\0';

    size_t len1 = strnlen(str1, sizeof(str1));
    if (len1 > 0 && str1[len1 - 1] == '\n') {
        str1[len1 - 1] = '\0';
    }

    size_t len2 = strnlen(str2, sizeof(str2));
    if (len2 > 0 && str2[len2 - 1] == '\n') {
        str2[len2 - 1] = '\0';
    }

    if (areIsomorphic(str1, str2, sizeof(str1))) {
        printf("The strings are isomorphic.\n");
    } else {
        printf("The strings are not isomorphic.\n");
    }

    return 0;
}