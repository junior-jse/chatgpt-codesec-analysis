#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *str) {
    size_t len;
    size_t i;
    size_t j;
    char temp;

    if (str == NULL) {
        return;
    }

    len = strlen(str);
    if (len == 0) {
        return;
    }

    i = 0;
    j = len - 1;

    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

void reverse_string_list(char **strings, size_t count) {
    size_t i;

    if (strings == NULL) {
        return;
    }

    for (i = 0; i < count; i++) {
        if (strings[i] != NULL) {
            reverse_string(strings[i]);
        }
    }
}

int main(void) {
    char str1[] = "Hello";
    char str2[] = "World";
    char str3[] = "Programming";
    char str4[] = "Language";
    char str5[] = "C";
    
    char *strings[] = {str1, str2, str3, str4, str5};
    size_t count = sizeof(strings) / sizeof(strings[0]);
    size_t i;

    printf("Original strings:\n");
    for (i = 0; i < count; i++) {
        printf("%s\n", strings[i]);
    }

    reverse_string_list(strings, count);

    printf("\nReversed strings:\n");
    for (i = 0; i < count; i++) {
        printf("%s\n", strings[i]);
    }

    return 0;
}