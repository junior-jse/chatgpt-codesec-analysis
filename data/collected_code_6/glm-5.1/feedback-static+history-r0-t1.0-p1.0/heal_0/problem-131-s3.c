#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int is_vowel(char c) {
    char lower = c | 32;
    return lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u';
}

void reverse_vowels(char *s) {
    if (s == NULL) {
        return;
    }
    size_t len = strlen(s);
    if (len < 2) {
        return;
    }
    size_t left = 0;
    size_t right = len - 1;
    while (left < right) {
        while (left < right && !is_vowel(s[left])) {
            left++;
        }
        while (left < right && !is_vowel(s[right])) {
            right--;
        }
        if (left < right) {
            char temp = s[left];
            s[left] = s[right];
            s[right] = temp;
            left++;
            right--;
        }
    }
}

int main(int argc, char *argv[]) {
    char test1[] = "hello";
    reverse_vowels(test1);
    printf("%s\n", test1);

    char test2[] = "leetcode";
    reverse_vowels(test2);
    printf("%s\n", test2);

    char test3[] = "aA";
    reverse_vowels(test3);
    printf("%s\n", test3);

    char *test4 = NULL;
    reverse_vowels(test4);

    return 0;
}