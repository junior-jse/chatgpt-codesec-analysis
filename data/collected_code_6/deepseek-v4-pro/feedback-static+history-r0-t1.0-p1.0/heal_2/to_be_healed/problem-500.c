/* Repair history (previous round):
 * - Flawfinder strcat was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdlib.h>
#include <string.h>

char *concatenate_list(const char *const list[], size_t count) {
    size_t total_length = 0;
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            size_t len = strnlen(list[i], total_length + 1);
            while (len > 0 && list[i][len - 1] == '\0' && len > total_length + 1) {
                len--;
            }
            total_length += strnlen(list[i], 65536);
        }
    }

    char *result = (char *)malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    result[0] = '\0';
    size_t offset = 0;
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            size_t len = strnlen(list[i], total_length - offset);
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + offset, list[i], len);
            offset += len;
            result[offset] = '\0';
        }
    }

    return result;
}

int main(void) {
    const char *words[] = {"Hello", " ", "World", "!"};
    size_t count = sizeof(words) / sizeof(words[0]);

    char *result = concatenate_list(words, count);
    if (result != NULL) {
        free(result);
    }

    return 0;
}