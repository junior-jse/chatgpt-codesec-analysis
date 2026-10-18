#include <stdio.h>
#include <string.h>

char find_first_repeated(const char *str) {
    if (str == NULL) {
        return '\0';
    }

    size_t len = strlen(str);
    if (len < 2) {
        return '\0';
    }

    unsigned char char_set[256] = {0};

    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (char_set[c] > 0) {
            return str[i];
        }
        char_set[c]++;
    }

    return '\0';
}

int main(int argc, char *argv[]) {
    const char *test_str1 = "programming";
    char res1 = find_first_repeated(test_str1);
    if (res1 != '\0') {
        printf("First repeated character in '%s': %c\n", test_str1, res1);
    } else {
        printf("No repeated characters found in '%s'\n", test_str1);
    }

    const char *test_str2 = "abcdef";
    char res2 = find_first_repeated(test_str2);
    if (res2 != '\0') {
        printf("First repeated character in '%s': %c\n", test_str2, res2);
    } else {
        printf("No repeated characters found in '%s'\n", test_str2);
    }

    const char *test_str3 = "";
    char res3 = find_first_repeated(test_str3);
    if (res3 != '\0') {
        printf("First repeated character in '%s': %c\n", test_str3, res3);
    } else {
        printf("No repeated characters found in '%s'\n", test_str3);
    }

    return 0;
}