#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t safe_strlen(const char *str, size_t max_size) {
    size_t len = 0;
    if (str == NULL) return 0;
    while (len < max_size && str[len] != '\0') {
        len++;
    }
    return len;
}

void reverse_string(char *str, size_t size) {
    if (str == NULL || size == 0) return;
    size_t length = safe_strlen(str, size);
    for (size_t i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - 1 - i];
        str[length - 1 - i] = temp;
    }
}

void reverse_strings(char **list, size_t count, size_t str_size) {
    if (list == NULL || count == 0) return;
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            reverse_string(list[i], str_size);
        }
    }
}

int main(void) {
    enum { NUM_STRINGS = 4, STR_LEN = 50 };
    char strings[NUM_STRINGS][STR_LEN] = {
        "hello",
        "world",
        "example",
        "programming"
    };
    char *list[NUM_STRINGS];
    
    for (size_t i = 0; i < NUM_STRINGS; i++) {
        list[i] = strings[i];
    }
    
    reverse_strings(list, NUM_STRINGS, STR_LEN);
    
    for (size_t i = 0; i < NUM_STRINGS; i++) {
        printf("%s\n", list[i]);
    }
    
    return 0;
}