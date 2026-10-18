#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

char find_first_repeated(const char *str, size_t len) {
    if (str == NULL) {
        return '\0';
    }

    if (len < 2) {
        return '\0';
    }

    unsigned char *char_set = calloc(256, sizeof(unsigned char));
    if (char_set == NULL) {
        return '\0';
    }

    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (c == '\0') {
            break;
        }
        if (char_set[c] > 0) {
            free(char_set);
            return (char)c;
        }
        char_set[c]++;
    }

    free(char_set);
    return '\0';
}

int main(int argc, char *argv[]) {
    const char test_str1[] = "programming";
    char res1 = find_first_repeated(test_str1, sizeof(test_str1) - 1);
    if (res1 != '\0') {
        printf("First repeated character in '%s': %c\n", test_str1, res1);
    } else {
        printf("No repeated characters found in '%s'\n", test_str1);
    }

    const char test_str2[] = "abcdef";
    char res2 = find_first_repeated(test_str2, sizeof(test_str2) - 1);
    if (res2 != '\0') {
        printf("First repeated character in '%s': %c\n", test_str2, res2);
    } else {
        printf("No repeated characters found in '%s'\n", test_str2);
    }

    const char test_str3[] = "";
    char res3 = find_first_repeated(test_str3, sizeof(test_str3) - 1);
    if (res3 != '\0') {
        printf("First repeated character in '%s': %c\n", test_str3, res3);
    } else {
        printf("No repeated characters found in '%s'\n", test_str3);
    }

    return 0;
}