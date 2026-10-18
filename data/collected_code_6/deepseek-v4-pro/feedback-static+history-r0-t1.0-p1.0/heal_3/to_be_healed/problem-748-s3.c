#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *insert_spaces(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    regex_t regex;
    regmatch_t matches[2];
    const char *pattern = "([a-z])([A-Z])";
    int ret;

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t input_len = strlen(input);
    size_t output_len = input_len * 2 + 1;
    char *output = (char *)malloc(output_len);
    if (output == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t output_pos = 0;
    size_t search_offset = 0;

    while (search_offset < input_len && regexec(&regex, input + search_offset, 2, matches, 0) == 0) {
        size_t match_start = search_offset + matches[0].rm_so;

        while (search_offset < match_start + 1 && search_offset < input_len) {
            output[output_pos++] = input[search_offset++];
        }

        if (output_pos < output_len - 1) {
            output[output_pos++] = ' ';
        }

        search_offset = match_start + 1;
    }

    while (search_offset < input_len && output_pos < output_len - 1) {
        output[output_pos++] = input[search_offset++];
    }

    output[output_pos] = '\0';
    regfree(&regex);
    return output;
}

int main(void) {
    const char *test_strings[] = {
        "HelloWorld",
        "ThisIsATestString",
        "noSpacesHere",
        "Already Spaced String",
        "MixtureOfUPPERAndlower",
        "",
        "A",
        "aB",
        "AB",
        NULL
    };

    for (int i = 0; test_strings[i] != NULL; i++) {
        char *result = insert_spaces(test_strings[i]);
        if (result != NULL) {
            printf("'%s' -> '%s'\n", test_strings[i], result);
            free(result);
        } else {
            printf("'%s' -> error\n", test_strings[i]);
        }
    }

    return 0;
}