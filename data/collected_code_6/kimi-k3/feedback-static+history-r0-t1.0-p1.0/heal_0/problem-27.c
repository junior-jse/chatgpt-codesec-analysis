#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void remove_digits(char *str) {
    if (str == NULL) {
        return;
    }
    
    size_t read_pos = 0;
    size_t write_pos = 0;
    size_t len = strlen(str);
    
    while (read_pos < len) {
        if (!isdigit((unsigned char)str[read_pos])) {
            str[write_pos] = str[read_pos];
            write_pos++;
        }
        read_pos++;
    }
    str[write_pos] = '\0';
}

void process_string_list(char **list, size_t count) {
    if (list == NULL) {
        return;
    }
    
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            remove_digits(list[i]);
        }
    }
}

int main(void) {
    char *strings[] = {
        "abc123def",
        "test456",
        "no_digits_here",
        "789mixed012",
        "999",
        ""
    };
    
    size_t count = sizeof(strings) / sizeof(strings[0]);
    
    printf("Original strings:\n");
    for (size_t i = 0; i < count; i++) {
        printf("[%zu]: %s\n", i, strings[i]);
    }
    
    process_string_list(strings, count);
    
    printf("\nAfter removing digits:\n");
    for (size_t i = 0; i < count; i++) {
        printf("[%zu]: %s\n", i, strings[i]);
    }
    
    return 0;
}