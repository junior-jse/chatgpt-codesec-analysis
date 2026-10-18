#include <stdio.h>
#include <stdlib.h>

void reverse_words(char *str) {
    if (str == NULL) return;

    const char *end = str;
    while (*end) ++end;
    if (end == str) return;

    const char *scan = str;
    size_t total_letters = 0;
    int word_count = 0;

    while (*scan) {
        while (*scan == ' ') scan++;
        if (*scan == '\0') break;
        const char *wstart = scan;
        while (*scan && *scan != ' ') scan++;
        total_letters += (size_t)(scan - wstart);
        word_count++;
    }

    if (word_count == 0) {
        str[0] = '\0';
        return;
    }

    size_t result_len = total_letters + (word_count - 1);
    char *temp = malloc(result_len + 1);
    if (temp == NULL) return;

    typedef struct {
        const char *start;
        size_t len;
    } word_t;

    word_t *words = malloc(word_count * sizeof(word_t));
    if (words == NULL) {
        free(temp);
        return;
    }

    scan = str;
    int idx = 0;
    while (*scan) {
        while (*scan == ' ') scan++;
        if (*scan == '\0') break;
        words[idx].start = scan;
        const char *wstart = scan;
        while (*scan && *scan != ' ') scan++;
        words[idx].len = (size_t)(scan - wstart);
        idx++;
    }

    char *dest = temp;
    for (int i = word_count - 1; i >= 0; i--) {
        if (i != word_count - 1) {
            *dest++ = ' ';
        }
        for (size_t j = 0; j < words[i].len; j++) {
            *dest++ = words[i].start[j];
        }
    }
    *dest = '\0';

    char *dst = str;
    for (size_t k = 0; k <= result_len; k++) {
        dst[k] = temp[k];
    }

    free(temp);
    free(words);
}

int main(void) {
    char str1[] = "Hello World";
    reverse_words(str1);
    printf("%s\n", str1);

    char str2[] = "This is a test";
    reverse_words(str2);
    printf("%s\n", str2);

    char str3[] = "Single";
    reverse_words(str3);
    printf("%s\n", str3);

    char str4[] = "";
    reverse_words(str4);
    printf("%s\n", str4);

    char str5[] = "   Multiple   spaces   ";
    reverse_words(str5);
    printf("%s\n", str5);

    return 0;
}