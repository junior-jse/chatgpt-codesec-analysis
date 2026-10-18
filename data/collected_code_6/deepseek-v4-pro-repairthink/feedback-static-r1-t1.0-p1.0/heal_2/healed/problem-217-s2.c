#include <stdio.h>
#include <string.h>

#define ASCII_SIZE 256
#define MAX_LEN 1024

char first_repeated_character(const char *str, size_t length) {
    if (str == NULL) {
        return '\0';
    }

    int count[ASCII_SIZE] = {0};
    size_t i;

    for (i = 0; i < length && str[i] != '\0'; i++) {
        count[(unsigned char)str[i]]++;
    }

    for (i = 0; i < length && str[i] != '\0'; i++) {
        if (count[(unsigned char)str[i]] > 1) {
            return str[i];
        }
    }

    return '\0';
}

int main(void) {
    const char *test_strings[] = {
        "hello",
        "programming",
        "abcdef",
        "",
        "aabbcc",
        NULL
    };

    for (int i = 0; test_strings[i] != NULL; i++) {
        size_t len = 0;
        while (len < MAX_LEN && test_strings[i][len] != '\0') {
            len++;
        }
        char result = first_repeated_character(test_strings[i], len);
        if (result != '\0') {
            printf("First repeated character in \"%s\": %c\n", test_strings[i], result);
        } else {
            printf("No repeated character in \"%s\"\n", test_strings[i]);
        }
    }

    return 0;
}