#include <stdio.h>
#include <string.h>

#define MAX_STRING_LEN 1024

static size_t safe_strlen(const char *s, size_t max_len)
{
    size_t len = 0;

    if (s == NULL) {
        return 0;
    }

    while (len < max_len && s[len] != '\0') {
        len++;
    }

    return len;
}

int isReverse(const char *s1, const char *s2)
{
    if (s1 == NULL || s2 == NULL) {
        return 0;
    }

    size_t len1 = safe_strlen(s1, MAX_STRING_LEN);
    size_t len2 = safe_strlen(s2, MAX_STRING_LEN);

    if (len1 >= MAX_STRING_LEN || len2 >= MAX_STRING_LEN) {
        return 0;
    }

    if (len1 != len2) {
        return 0;
    }

    for (size_t i = 0; i < len1; i++) {
        if (s1[i] != s2[len1 - 1 - i]) {
            return 0;
        }
    }

    return 1;
}

int countReversePairs(const char * const list[], int size)
{
    int count = 0;

    if (list == NULL || size <= 0) {
        return 0;
    }

    for (int i = 0; i < size; i++) {
        if (list[i] == NULL) {
            continue;
        }
        for (int j = i + 1; j < size; j++) {
            if (list[j] == NULL) {
                continue;
            }
            if (isReverse(list[i], list[j])) {
                count++;
            }
        }
    }

    return count;
}

int main(void)
{
    const char *list[] = {"abc", "def", "cba", "fed", "ghi"};
    int size = (int)(sizeof(list) / sizeof(list[0]));

    int result = countReversePairs(list, size);
    printf("Number of reverse pairs: %d\n", result);

    return 0;
}