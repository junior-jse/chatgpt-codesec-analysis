#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *str) {
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t length = strlen(str);
    for (size_t i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - 1 - i];
        str[length - 1 - i] = temp;
    }
}

void reverse_strings(char **list, size_t count) {
    if (list == NULL || count == 0) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            reverse_string(list[i]);
        }
    }
}

int main(void) {
    char strings[][50] = {
        "hello",
        "world",
        "example",
        "programming"
    };
    size_t count = sizeof(strings) / sizeof(strings[0]);
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char *list[count];
    
    for (size_t i = 0; i < count; i++) {
        list[i] = strings[i];
    }
    
    reverse_strings(list, count);
    
    for (size_t i = 0; i < count; i++) {
        printf("%s\n", list[i]);
    }
    
    return 0;
}