#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *snake_to_camel(const char *input)
{
    regex_t regex;
    regmatch_t matches[2];
    const char *cursor;
    char *output;
    size_t output_len;
    size_t input_len;
    size_t i;
    size_t j;
    int ret;

    if (input == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    input_len = strlen(input);
    output_len = input_len + 1;
    output = (char *)malloc(output_len);
    if (output == NULL) {
        return NULL;
    }

    ret = regcomp(&regex, "_([a-z])", REG_EXTENDED);
    if (ret != 0) {
        free(output);
        return NULL;
    }

    i = 0;
    /* Possible weaknesses found:
     *  Variable 'j' is assigned a value that is never used. [unreadVariable]
     *  Value stored to 'j' is never read [deadcode.DeadStores]
     */
    j = 0;
    cursor = input;

    while (*cursor != '\0') {
        ret = regexec(&regex, cursor, 2, matches, 0);
        if (ret == 0 && matches[1].rm_so >= 0) {
            size_t prefix_len = (size_t)matches[0].rm_so;
            /* Possible weaknesses found:
             *  Value stored to 'match_len' during its initialization is never read [deadcode.DeadStores]
             *  Variable 'match_len' is assigned a value that is never used. [unreadVariable]
             */
            size_t match_len = (size_t)(matches[0].rm_eo - matches[0].rm_so);
            char upper_char;

            if (i + prefix_len >= output_len) {
                regfree(&regex);
                free(output);
                return NULL;
            }

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(output + i, cursor, prefix_len);
            i += prefix_len;

            upper_char = cursor[matches[1].rm_so];
            if (upper_char >= 'a' && upper_char <= 'z') {
                upper_char = (char)(upper_char - 'a' + 'A');
            }

            if (i + 1 >= output_len) {
                regfree(&regex);
                free(output);
                return NULL;
            }

            output[i] = upper_char;
            i++;

            cursor += matches[0].rm_eo;
        } else {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t remaining = strlen(cursor);

            if (i + remaining >= output_len) {
                regfree(&regex);
                free(output);
                return NULL;
            }

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(output + i, cursor, remaining);
            i += remaining;
            break;
        }
    }

    output[i] = '\0';
    regfree(&regex);
    return output;
}

int main(void)
{
    const char *input = "hello_world_this_is_c";
    char *result = snake_to_camel(input);

    if (result == NULL) {
        return 1;
    }

    printf("%s\n", result);
    free(result);
    return 0;
}