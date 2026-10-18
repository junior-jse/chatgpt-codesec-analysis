#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_word_boundary(unsigned char c)
{
    return c == '\0' || !isalnum(c);
}

static char *abbreviate_road(const char *input)
{
    const size_t word_length = 4U;
    const char replacement[] = "rd.";
    const size_t replacement_length = sizeof(replacement) - 1U;
    size_t input_length;
    size_t match_count = 0U;
    size_t output_length;
    size_t i;
    size_t j;
    char *output;

    if (input == NULL) {
        return NULL;
    }

    input_length = strlen(input);

    for (i = 0U; i + word_length <= input_length; ++i) {
        if ((i == 0U || is_word_boundary((unsigned char)input[i - 1U])) &&
            tolower((unsigned char)input[i]) == 'r' &&
            tolower((unsigned char)input[i + 1U]) == 'o' &&
            tolower((unsigned char)input[i + 2U]) == 'a' &&
            tolower((unsigned char)input[i + 3U]) == 'd' &&
            is_word_boundary((unsigned char)input[i + word_length])) {
            ++match_count;
            i += word_length - 1U;
        }
    }

    output_length = input_length - match_count * (word_length - replacement_length);
    output = malloc(output_length + 1U);
    if (output == NULL) {
        return NULL;
    }

    i = 0U;
    j = 0U;

    while (i < input_length) {
        if (i + word_length <= input_length &&
            (i == 0U || is_word_boundary((unsigned char)input[i - 1U])) &&
            tolower((unsigned char)input[i]) == 'r' &&
            tolower((unsigned char)input[i + 1U]) == 'o' &&
            tolower((unsigned char)input[i + 2U]) == 'a' &&
            tolower((unsigned char)input[i + 3U]) == 'd' &&
            is_word_boundary((unsigned char)input[i + word_length])) {
            memcpy(output + j, replacement, replacement_length);
            i += word_length;
            j += replacement_length;
        } else {
            output[j++] = input[i++];
        }
    }

    output[j] = '\0';
    return output;
}

int main(void)
{
    char input[1024];
    char *result;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        if (ferror(stdin)) {
            fprintf(stderr, "Failed to read input.\n");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    input[strcspn(input, "\n")] = '\0';

    result = abbreviate_road(input);
    if (result == NULL) {
        fprintf(stderr, "Failed to abbreviate input.\n");
        return EXIT_FAILURE;
    }

    if (printf("%s\n", result) < 0) {
        free(result);
        return EXIT_FAILURE;
    }

    free(result);
    return EXIT_SUCCESS;
}