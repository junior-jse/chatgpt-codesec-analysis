#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int is_vowel(char c) {
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

void reverse_vowels(char *s) {
    if (s == NULL) {
        return;
    }
    
    size_t left = 0;
    size_t right = strlen(s);
    
    if (right == 0) {
        return;
    }
    
    right--;
    
    while (left < right) {
        if (!is_vowel(s[left])) {
            left++;
        } else if (!is_vowel(s[right])) {
            right--;
        } else {
            char temp = s[left];
            s[left] = s[right];
            s[right] = temp;
            left++;
            right--;
        }
    }
}

int main(void) {
    char str1[] = "hello world";
    char str2[] = "programming";
    char str3[] = "AEIOU";
    
    printf("Original: %s\n", str1);
    reverse_vowels(str1);
    printf("Reversed: %s\n", str1);
    
    printf("Original: %s\n", str2);
    reverse_vowels(str2);
    printf("Reversed: %s\n", str2);
    
    printf("Original: %s\n", str3);
    reverse_vowels(str3);
    printf("Reversed: %s\n", str3);
    
    return 0;
}