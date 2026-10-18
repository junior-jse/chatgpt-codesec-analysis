#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int is_vowel(char c) {
    c = tolower((unsigned char)c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

static size_t safe_strlen(const char *str, size_t maxlen) {
    size_t len = 0;
    while (len < maxlen && str[len] != '\0') {
        len++;
    }
    return len;
}

void reverse_vowels(char *str) {
    if (str == NULL) {
        return;
    }

    size_t len = safe_strlen(str, 4096);
    if (len == 0) {
        return;
    }

    size_t left = 0;
    size_t right = len - 1;

    while (left < right) {
        while (left < right && !is_vowel(str[left])) {
            left++;
        }
        while (left < right && !is_vowel(str[right])) {
            right--;
        }
        if (left < right) {
            char temp = str[left];
            str[left] = str[right];
            str[right] = temp;
            left++;
            right--;
        }
    }
}

int main(void) {
    char str1[] = "hello";
    char str2[] = "leetcode";
    char str3[] = "aA";
    char str4[] = "";
    char str5[] = "xyz";

    reverse_vowels(str1);
    reverse_vowels(str2);
    reverse_vowels(str3);
    reverse_vowels(str4);
    reverse_vowels(str5);

    printf("%s\n", str1);
    printf("%s\n", str2);
    printf("%s\n", str3);
    printf("%s\n", str4);
    printf("%s\n", str5);

    return 0;
}