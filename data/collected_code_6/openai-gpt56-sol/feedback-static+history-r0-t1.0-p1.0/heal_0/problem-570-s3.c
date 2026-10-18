#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void free_string_list(char **list, size_t count)
{
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(list[i]);
    }

    free(list);
}

static char *remove_matching_words(const char *input, const char *pattern)
{
    size_t input_len;
    char *output;
    size_t read_pos = 0;
    size_t write_pos = 0;

    if (input == NULL || pattern == NULL) {
        return NULL;
    }

    input_len = strlen(input);
    output = malloc(input_len + 1);
    if (output == NULL) {
        return NULL;
    }

    while (read_pos < input_len) {
        size_t separator_start = read_pos;
        size_t word_start;
        size_t word_end;

        while (read_pos < input_len &&
               (input[read_pos] == ' ' || input[read_pos] == '\t' ||
                input[read_pos] == '\n' || input[read_pos] == '\r' ||
                input[read_pos] == '\f' || input[read_pos] == '\v')) {
            ++read_pos;
        }

        word_start = read_pos;

        while (read_pos < input_len &&
               input[read_pos] != ' ' && input[read_pos] != '\t' &&
               input[read_pos] != '\n' && input[read_pos] != '\r' &&
               input[read_pos] != '\f' && input[read_pos] != '\v') {
            ++read_pos;
        }

        word_end = read_pos;

        if (word_start == word_end) {
            memcpy(output + write_pos, input + separator_start,
                   word_start - separator_start);
            write_pos += word_start - separator_start;
            break;
        }

        {
            size_t word_len = word_end - word_start;
            char saved = input[word_end];
            int matches;
            char *mutable_input = (char *)input;

            mutable_input[word_end] = '\0';
            matches = strstr(input + word_start, pattern) != NULL;
            mutable_input[word_end] = saved;

            if (!matches) {
                size_t separator_len = word_start - separator_start;
                memcpy(output + write_pos, input + separator_start,
                       separator_len);
                write_pos += separator_len;
                memcpy(output + write_pos, input + word_start, word_len);
                write_pos += word_len;
            }
        }
    }

    output[write_pos] = '\0';
    return output;
}

static char **filter_string_list(const char *const *list, size_t count,
                                 const char *pattern)
{
    char **result;

    if ((list == NULL && count != 0) || pattern == NULL) {
        return NULL;
    }

    if (count > SIZE_MAX / sizeof(*result)) {
        return NULL;
    }

    result = calloc(count == 0 ? 1 : count, sizeof(*result));
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        if (list[i] == NULL) {
            free_string_list(result, i);
            return NULL;
        }

        result[i] = remove_matching_words(list[i], pattern);
        if (result[i] == NULL) {
            free_string_list(result, i);
            return NULL;
        }
    }

    return result;
}

int main(void)
{
    const char *strings[] = {
        "apple banana apricot cherry",
        "cat dog caterpillar bird",
        "alpha beta gamma delta"
    };
    const char pattern[] = "cat";
    const size_t count = sizeof(strings) / sizeof(strings[0]);
    char **filtered = filter_string_list(strings, count, pattern);

    if (filtered == NULL) {
        fputs("Failed to filter strings.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; ++i) {
        if (puts(filtered[i]) == EOF) {
            free_string_list(filtered, count);
            return EXIT_FAILURE;
        }
    }

    free_string_list(filtered, count);
    return EXIT_SUCCESS;
}