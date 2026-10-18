#include <stdio.h>
#include <string.h>

int min_swaps(const char *str1, size_t len1, const char *str2, size_t len2) {
    if (len1 != len2) {
        return -1;
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
    char str1[100], str2[100];
    
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
    
    size_t len1 = strnlen(str1, sizeof(str1));
    size_t len2 = strnlen(str2, sizeof(str2));
    
    int result = min_swaps(str1, len1, str2, len2);
    
    if (result == -1) {
        printf("Conversion is not possible\n");
    } else {
        printf("Minimum swaps required: %d\n", result);
    }
    
    return 0;
}