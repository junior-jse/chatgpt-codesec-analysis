#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *abbreviate_road(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0, j = 0;
    while (i < len) {
        if (str[i] == 'r' || str[i] == 'R') {
            size_t remaining = len - i;
            if (remaining >= 4) {
                if ((str[i + 1] == 'o' || str[i + 1] == 'O') &&
                    (str[i + 2] == 'a' || str[i + 2] == 'A') &&
                    (str[i + 3] == 'd' || str[i + 3] == 'D')) {
                    result[j++] = str[i];
                    result[j++] = (str[i + 1] == 'O') ? 'D' : 'd';
                    result[j++] = '.';
                    i += 4;
                    continue;
                }
            }
        }
        result[j++] = str[i++];
    }
    result[j] = '\0';

    char *shrunk = realloc(result, j + 1);
    if (shrunk != NULL) {
        result = shrunk;
    }

    return result;
}

int main(void) {
    char *s1 = abbreviate_road("I walked down the Road");
    if (s1 != NULL) {
        printf("%s\n", s1);
        free(s1);
    }

    char *s2 = abbreviate_road("road ahead");
    if (s2 != NULL) {
        printf("%s\n", s2);
        free(s2);
    }

    char *s3 = abbreviate_road(NULL);
    if (s3 == NULL) {
        printf("NULL input handled\n");
    }

    return 0;
}