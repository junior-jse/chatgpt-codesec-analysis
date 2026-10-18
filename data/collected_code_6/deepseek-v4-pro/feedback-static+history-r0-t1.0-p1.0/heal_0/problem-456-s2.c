#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *str) {
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