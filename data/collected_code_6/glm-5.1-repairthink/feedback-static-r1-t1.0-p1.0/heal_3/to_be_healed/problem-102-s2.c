#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

char *snake_to_camel(const char *snake_str) {
    if (snake_str == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(snake_str);
    if (len == SIZE_MAX) {
        return NULL;
    }

    char *camel_str = malloc(len + 1);
    if (camel_str == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (snake_str[i] == '_') {
            if (i + 1 < len && islower((unsigned char)snake_str[i + 1])) {
                camel_str[j++] = (char)toupper((unsigned char)snake_str[i + 1]);
                i++;
            } else {
                camel_str[j++] = snake_str[i];
            }
        } else {
            camel_str[j++] = snake_str[i];
        }
    }
    camel_str[j] = '\0';

    char *final_str = realloc(camel_str, j + 1);
    if (final_str == NULL) {
        return camel_str;
    }

    return final_str;
}

int main(void) {
    const char *inputs[] = {
        "hello_world",
        "snake_case_string",
        "alreadycamel",
        "_leading_underscore",
        "trailing_underscore_",
        "multiple___underscores",
        "mixed_Case_String",
        ""
    };

    size_t num_inputs = sizeof(inputs) / sizeof(inputs[0]);

    for (size_t i = 0; i < num_inputs; i++) {
        char *result = snake_to_camel(inputs[i]);
        if (result != NULL) {
            printf("Input: \"%s\" -> Output: \"%s\"\n", inputs[i], result);
            free(result);
        } else {
            printf("Input: \"%s\" -> Output: Allocation failed\n", inputs[i]);
        }
    }

    snake_to_camel(NULL);
    printf("NULL input handled correctly.\n");

    return 0;
}