#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHARS 256

char firstRepeatedChar(const char *str) {
    if (str == NULL) {
        return '\0';
    }

    bool seen[MAX_CHARS] = {false};
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++) {
        unsigned char ch = (unsigned char)str[i];
        if (seen[ch]) {
            return str[i];
        }
        seen[ch] = true;
    }

    return '\0';
}

int main(void) {
    const char *testStrings[] = {
        "abcdefgha",
        "hello",
        "abcabc",
        "testing",
        "unique"
    };

    for (size_t i = 0; i < sizeof(testStrings) / sizeof(testStrings[0]); i++) {
        char result = firstRepeatedChar(testStrings[i]);
        printf("First repeated character in \"%s\": ", testStrings[i]);
        if (result != '\0') {
            printf("%c\n", result);
        } else {
            printf("None\n");
        }
    }

    return 0;
}