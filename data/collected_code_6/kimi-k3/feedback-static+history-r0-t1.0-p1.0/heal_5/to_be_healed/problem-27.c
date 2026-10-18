#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void remove_digits(char *str, size_t max_len) {
    if (str == NULL || max_len == 0) {
        return;
    }
    
    size_t read_pos = 0;
    size_t write_pos = 0;
    size_t len = strnlen(str, max_len);
    
    while (read_pos < len) {
        if (!isdigit((unsigned char)str[read_pos])) {
            str[write_pos] = str[read_pos];
            write_pos++;
        }
        read_pos++;
    }
    str[write_pos] = '\0';
}

/* Possible weaknesses found:
 * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
 */
void process_string_list(char (*list)[64], size_t count, size_t max_len) {
    if (list == NULL || max_len == 0) {
        return;
    }
    
    for (size_t i = 0; i < count; i++) {
        remove_digits(list[i], max_len);
    }
}

int main(void) {
    char strings[][64] = {
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
    
    process_string_list(strings, count, sizeof(strings[0]));
    
    printf("\nAfter removing digits:\n");
    for (size_t i = 0; i < count; i++) {
        printf("[%zu]: %s\n", i, strings[i]);
    }
    
    return 0;
}