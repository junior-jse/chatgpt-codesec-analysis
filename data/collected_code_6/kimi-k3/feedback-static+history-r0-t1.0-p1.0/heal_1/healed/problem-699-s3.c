#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int min_swaps(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) {
        return -1;
    }
    
    size_t len1 = strnlen(s1, 1000);
    size_t len2 = strnlen(s2, 1000);
    
    if (len1 != len2) {
        return -1;
    }
    
    if (len1 == 1000 && s1[999] != '\0') {
        return -1;
    }
    if (len2 == 1000 && s2[999] != '\0') {
        return -1;
    }
    
    int count1_s1 = 0, count1_s2 = 0;
    size_t i;
    
    for (i = 0; i < len1; i++) {
        if (s1[i] != '0' && s1[i] != '1') {
            return -1;
        }
        if (s2[i] != '0' && s2[i] != '1') {
            return -1;
        }
        if (s1[i] == '1') count1_s1++;
        if (s2[i] == '1') count1_s2++;
    }
    
    if (count1_s1 != count1_s2) {
        return -1;
    }
    
    int swaps = 0;
    int diff = 0;
    
    for (i = 0; i < len1; i++) {
        if (s1[i] != s2[i]) {
            if (s1[i] == '1') {
                diff++;
            } else {
                diff--;
            }
            if (diff > 0) {
                swaps += diff;
            }
        }
    }
    
    return swaps;
}

int main(void) {
    char s1[1000], s2[1000];
    size_t len;
    
    if (fgets(s1, sizeof(s1), stdin) == NULL) {
        return 1;
    }
    if (fgets(s2, sizeof(s2), stdin) == NULL) {
        return 1;
    }
    
    len = strcspn(s1, "\n");
    s1[len] = '\0';
    
    len = strcspn(s2, "\n");
    s2[len] = '\0';
    
    if (strnlen(s1, sizeof(s1)) == sizeof(s1)) {
        return 1;
    }
    if (strnlen(s2, sizeof(s2)) == sizeof(s2)) {
        return 1;
    }
    
    int result = min_swaps(s1, s2);
    printf("%d\n", result);
    
    return 0;
}