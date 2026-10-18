#include <stdio.h>
#include <string.h>

char find_max_occurring_char(const char *str) {
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    if (str == NULL || strlen(str) == 0) {
        return '\0';
    }

    int count[256] = {0};
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++) {
        unsigned char c = str[i];
        count[c]++;
    }

    int max_count = -1;
    char max_char = '\0';

    for (size_t i = 0; i < len; i++) {
        unsigned char c = str[i];
        if (count[c] > max_count) {
            max_count = count[c];
            max_char = str[i];
        }
    }

    return max_char;
}

int main(void) {
    const char *str = "experience";
    char result = find_max_occurring_char(str);

    if (result != '\0') {
        printf("Maximum occurring character: %c\n", result);
    } else {
        printf("String is empty or NULL.\n");
    }

    return 0;
}