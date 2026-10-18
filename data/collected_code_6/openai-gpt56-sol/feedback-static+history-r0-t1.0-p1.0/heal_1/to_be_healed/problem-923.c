#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t shortest_common_supersequence_length(const char *str1, const char *str2)
{
    size_t len1;
    size_t len2;
    size_t *previous;
    size_t *current;
    size_t i;
    size_t j;
    size_t lcs_length;
    size_t result;

    if (str1 == NULL || str2 == NULL) {
        return SIZE_MAX;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len1 = strlen(str1);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len2 = strlen(str2);

    if (len1 > SIZE_MAX - len2) {
        return SIZE_MAX;
    }

    if (len2 > (SIZE_MAX / sizeof(*previous)) - 1U) {
        return SIZE_MAX;
    }

    previous = calloc(len2 + 1U, sizeof(*previous));
    current = calloc(len2 + 1U, sizeof(*current));

    if (previous == NULL || current == NULL) {
        free(previous);
        free(current);
        return SIZE_MAX;
    }

    for (i = 1U; i <= len1; ++i) {
        current[0] = 0U;

        for (j = 1U; j <= len2; ++j) {
            if (str1[i - 1U] == str2[j - 1U]) {
                current[j] = previous[j - 1U] + 1U;
            } else {
                current[j] = previous[j] > current[j - 1U]
                                 ? previous[j]
                                 : current[j - 1U];
            }
        }

        {
            size_t *temporary = previous;
            previous = current;
            current = temporary;
        }
    }

    lcs_length = previous[len2];
    result = len1 + len2 - lcs_length;

    free(previous);
    free(current);

    return result;
}

int main(void)
{
    const char *str1 = "AGGTAB";
    const char *str2 = "GXTXAYB";
    size_t length = shortest_common_supersequence_length(str1, str2);

    if (length == SIZE_MAX) {
        fputs("Unable to calculate the length.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%zu\n", length) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}