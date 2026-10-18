#include <stdio.h>
#include <string.h>

char find_first_repeated(const char *str, size_t max_len) {
    if (str == NULL || max_len == 0) {
        return '\0';
    }

    size_t len = strnlen(str, max_len);
    if (len < 2) {
        return '\0';
    }

    if (len > 256) {
        len = 256;
    }

    unsigned char char_set[256] = {0};

    for (size_t i = 0; i < len; i++) {
        size_t idx = (unsigned char)str[i];
        if (char_set[idx] > 0) {
            return str[i];
        }
        char_set[idx]++;
    }

    return '\0';
}

int main(int argc, char *argv[]) {
    size_t max_len = 1024;

    const char *test_str1 = "programming";
    char res1 = find_first_repeated(test_str1, max_len);
    if (res1 != '\0') {
        printf("First repeated character in '%s': %c\n", test_str1, res1);
    } else {
        printf("No repeated characters found in '%s'\n", test_str1);
    }

    const char *test_str2 = "abcdef";
    char res2 = find_first_repeated(test_str2, max_len);
    if (res2 != '\0') {
        printf("First repeated character in '%s': %c\n", test_str2, res2);
    } else {
        printf("No repeated characters found in '%s'\n", test_str2);
    }

    const char *test_str3 = "";
    char res3 = find_first_repeated(test_str3, max_len);
    if (res3 != '\0') {
        printf("First repeated character in '%s': %c\n", test_str3, res3);
    } else {
        printf("No repeated characters found in '%s'\n", test_str3);
    }

    return 0;
}