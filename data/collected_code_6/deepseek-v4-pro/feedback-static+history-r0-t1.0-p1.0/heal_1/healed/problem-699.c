#include <stdio.h>
#include <string.h>
#include <stddef.h>

int min_swaps(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) {
        return -1;
    }
    
    size_t len1 = strnlen(str1, 100);
    size_t len2 = strnlen(str2, 100);
    
    if (len1 != len2) {
        return -1;
    }
    
    if (len1 == 0) {
        return 0;
    }
    
    int count1 = 0, count2 = 0;
    for (size_t i = 0; i < len1; i++) {
        if (str1[i] == '1') count1++;
        if (str2[i] == '1') count2++;
    }
    
    if (count1 != count2) {
        return -1;
    }
    
    int mismatch = 0;
    for (size_t i = 0; i < len1; i++) {
        if (str1[i] != str2[i]) {
            mismatch++;
        }
    }
    
    return mismatch / 2;
}

int main(void) {
    char str1[101] = {0};
    char str2[101] = {0};
    
    printf("Enter first binary string: ");
    if (fgets(str1, sizeof(str1), stdin) == NULL) {
        return 1;
    }
    
    printf("Enter second binary string: ");
    if (fgets(str2, sizeof(str2), stdin) == NULL) {
        return 1;
    }
    
    str1[strcspn(str1, "\n")] = '\0';
    str2[strcspn(str2, "\n")] = '\0';
    
    int result = min_swaps(str1, str2);
    
    if (result == -1) {
        printf("Conversion is not possible\n");
    } else {
        printf("Minimum swaps required: %d\n", result);
    }
    
    return 0;
}