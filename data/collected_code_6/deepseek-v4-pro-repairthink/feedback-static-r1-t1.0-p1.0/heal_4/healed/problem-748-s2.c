#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *add_spaces_before_capitals(const char *input, size_t input_len) {
    if (input == NULL) {
        return NULL;
    }

    size_t spaces_needed = 0;
    for (size_t i = 0; i < input_len; i++) {
        unsigned char c = (unsigned char)input[i];
        if (i > 0 && isupper(c) && input[i - 1] != ' ') {
            spaces_needed++;
        }
    }

    size_t result_len = input_len + spaces_needed + 1;
    char *result = malloc(result_len);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < input_len; i++) {
        unsigned char c = (unsigned char)input[i];
        if (i > 0 && isupper(c) && input[i - 1] != ' ') {
            result[j++] = ' ';
        }
        result[j++] = (char)c;
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    char test1[] = "HelloWorldExample";
    char test2[] = "thisIsASampleString";
    char test3[] = "already separated words";
    char test4[] = "";
    char test5[] = "UPPERCASE";
    char test6[] = "noSpacesHere";

    char *result1 = add_spaces_before_capitals(test1, sizeof(test1) - 1);
    char *result2 = add_spaces_before_capitals(test2, sizeof(test2) - 1);
    char *result3 = add_spaces_before_capitals(test3, sizeof(test3) - 1);
    char *result4 = add_spaces_before_capitals(test4, sizeof(test4) - 1);
    char *result5 = add_spaces_before_capitals(test5, sizeof(test5) - 1);
    char *result6 = add_spaces_before_capitals(test6, sizeof(test6) - 1);

    printf("Input: %s\nOutput: %s\n\n", test1, result1 ? result1 : "NULL");
    printf("Input: %s\nOutput: %s\n\n", test2, result2 ? result2 : "NULL");
    printf("Input: %s\nOutput: %s\n\n", test3, result3 ? result3 : "NULL");
    printf("Input: %s\nOutput: %s\n\n", test4, result4 ? result4 : "NULL");
    printf("Input: %s\nOutput: %s\n\n", test5, result5 ? result5 : "NULL");
    printf("Input: %s\nOutput: %s\n\n", test6, result6 ? result6 : "NULL");

    free(result1);
    free(result2);
    free(result3);
    free(result4);
    free(result5);
    free(result6);

    return 0;
}