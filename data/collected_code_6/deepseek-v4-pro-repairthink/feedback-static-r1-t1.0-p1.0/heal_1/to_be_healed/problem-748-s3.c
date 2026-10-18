#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *insert_spaces(const char *input) {
    regex_t regex;
    regmatch_t matches[2];
    const char *pattern = "([a-z])([A-Z])";
    int ret;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t input_len = strlen(input);
    size_t output_len = input_len + 1;
    char *output = NULL;
    char *temp = NULL;
    const char *search_ptr = input;
    size_t offset = 0;
    /* Possible weaknesses found:
     *  Variable 'copy_len' is assigned a value that is never used. [unreadVariable]
     *  The scope of the variable 'copy_len' can be reduced. [variableScope]
     */
    size_t copy_len = 0;

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    output = (char *)malloc(output_len);
    if (output == NULL) {
        regfree(&regex);
        return NULL;
    }
    output[0] = '\0';

    while (regexec(&regex, search_ptr, 2, matches, 0) == 0) {
        copy_len = matches[0].rm_so + 1;
        output_len += 1;
        temp = (char *)realloc(output, output_len);
        if (temp == NULL) {
            free(output);
            regfree(&regex);
            return NULL;
        }
        output = temp;

        /* Possible weaknesses found:
         * Flawfinder strncat: Easily used incorrectly (e.g., incorrectly computing the correct maximum size to add) [MS-banned] (CWE-120). Consider strcat_s, strlcat, snprintf, or automatically resizing strings. (risk 1, buffer)
         */
        strncat(output, search_ptr, copy_len);
        /* Possible weaknesses found:
         * Flawfinder strcat: Does not check for buffer overflows when concatenating to destination [MS-banned] (CWE-120). Consider using strcat_s, strncat, strlcat, or snprintf (warning: strncat is easily misused). Risk is low because the source is a constant character. (risk 1, buffer)
         */
        strcat(output, " ");

        offset += matches[0].rm_eo;
        search_ptr = input + offset;
    }

    /* Possible weaknesses found:
     * Flawfinder strcat: Does not check for buffer overflows when concatenating to destination [MS-banned] (CWE-120). Consider using strcat_s, strncat, strlcat, or snprintf (warning: strncat is easily misused). (risk 4, buffer)
     */
    strcat(output, search_ptr);
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