#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *abbreviate_road(const char *input)
{
    static const char target[] = "road";
    static const char replacement[] = "rd.";
    const size_t target_len = sizeof(target) - 1;
    const size_t replacement_len = sizeof(replacement) - 1;
    size_t input_len;
    size_t occurrences = 0;
    size_t output_len;
    const char *cursor;
    char *output;
    char *dest;

    if (input == NULL) {
        return NULL;
    }

    input_len = strlen(input);
    cursor = input;

    while ((cursor = strstr(cursor, target)) != NULL) {
        ++occurrences;
        cursor += target_len;
    }

    output_len = input_len - occurrences * (target_len - replacement_len);
    output = malloc(output_len + 1);

    if (output == NULL) {
        return NULL;
    }

    cursor = input;
    dest = output;

    while (*cursor != '\0') {
        if (strncmp(cursor, target, target_len) == 0) {
            memcpy(dest, replacement, replacement_len);
            dest += replacement_len;
            cursor += target_len;
        } else {
            *dest++ = *cursor++;
        }
    }

    *dest = '\0';
    return output;
}

int main(void)
{
    char input[1024];
    char *result;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        if (ferror(stdin)) {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    input[strcspn(input, "\n")] = '\0';

    result = abbreviate_road(input);
    if (result == NULL) {
        return EXIT_FAILURE;
    }

    if (printf("%s\n", result) < 0) {
        free(result);
        return EXIT_FAILURE;
    }

    free(result);
    return EXIT_SUCCESS;
}