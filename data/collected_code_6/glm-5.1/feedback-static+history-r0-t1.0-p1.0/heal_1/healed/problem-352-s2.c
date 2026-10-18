#include <stdio.h>
#include <stdbool.h>

bool are_chars_unique(const char *str, size_t max_len) {
    if (str == NULL) {
        return false;
    }

    bool char_set[256] = {false};

    for (size_t i = 0; i < max_len; i++) {
        if (str[i] == '\0') {
            break;
        }
        unsigned char c = (unsigned char)str[i];
        if (char_set[c]) {
            return false;
        }
        char_set[c] = true;
    }

    return true;
}

int main(void) {
    const char *test1 = "abcdef";
    const char *test2 = "hello";
    const char *test3 = NULL;
    const char *test4 = "";

    printf("%d\n", are_chars_unique(test1, 256));
    printf("%d\n", are_chars_unique(test2, 256));
    printf("%d\n", are_chars_unique(test3, 256));
    printf("%d\n", are_chars_unique(test4, 256));

    return 0;
}