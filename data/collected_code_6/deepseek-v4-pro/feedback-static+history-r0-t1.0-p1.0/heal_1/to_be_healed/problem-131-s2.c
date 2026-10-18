#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isVowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

char* reverseVowels(char* str) {
    if (str == NULL) {
        return NULL;
    }
    
    int left = 0;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    int right = strlen(str) - 1;
    
    while (left < right) {
        while (left < right && !isVowel(str[left])) {
            left++;
        }
        while (left < right && !isVowel(str[right])) {
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
    
    return str;
}

int main(void) {
    char str1[] = "hello";
    char str2[] = "leetcode";
    char str3[] = "aeiou";
    char str4[] = "bcdfg";
    
    printf("Original: %s\n", str1);
    reverseVowels(str1);
    printf("Reversed vowels: %s\n\n", str1);
    
    printf("Original: %s\n", str2);
    reverseVowels(str2);
    printf("Reversed vowels: %s\n\n", str2);
    
    printf("Original: %s\n", str3);
    reverseVowels(str3);
    printf("Reversed vowels: %s\n\n", str3);
    
    printf("Original: %s\n", str4);
    reverseVowels(str4);
    printf("Reversed vowels: %s\n\n", str4);
    
    return 0;
}