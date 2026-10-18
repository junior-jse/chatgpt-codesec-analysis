#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_vowel(char c) {
    c = tolower((unsigned char)c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

void reverse_vowels(char *str) {
    if (str == NULL) {
        return;
    }
    
    size_t left = 0;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t right = strlen(str);
    
    if (right == 0) {
        return;
    }
    
    right--;
    
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
    char str1[] = "hello world";
    char str2[] = "programming";
    char str3[] = "aAeEiIoOuU";
    char str4[] = "xyz";
    char str5[] = "";
    
    printf("Original: %s\n", str1);
    reverse_vowels(str1);
    printf("Reversed vowels: %s\n\n", str1);
    
    printf("Original: %s\n", str2);
    reverse_vowels(str2);
    printf("Reversed vowels: %s\n\n", str2);
    
    printf("Original: %s\n", str3);
    reverse_vowels(str3);
    printf("Reversed vowels: %s\n\n", str3);
    
    printf("Original: %s\n", str4);
    reverse_vowels(str4);
    printf("Reversed vowels: %s\n\n", str4);
    
    printf("Original: %s\n", str5);
    reverse_vowels(str5);
    printf("Reversed vowels: %s\n", str5);
    
    return 0;
}