#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isReverse(const char *s1, const char *s2) {
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len1 = strlen(s1);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len2 = strlen(s2);
    
    if (len1 != len2) {
        return 0;
    }
    
    for (size_t i = 0; i < len1; i++) {
        if (s1[i] != s2[len1 - 1 - i]) {
            return 0;
        }
    }
    
    return 1;
}

/* Possible weaknesses found:
 *  Parameter 'list' can be declared as const array [constParameter]
 */
int countReversePairs(char *list[], int size) {
    int count = 0;
    
    if (list == NULL || size <= 0) {
        return 0;
    }
    
    for (int i = 0; i < size; i++) {
        if (list[i] == NULL) {
            continue;
        }
        for (int j = i + 1; j < size; j++) {
            if (list[j] == NULL) {
                continue;
            }
            if (isReverse(list[i], list[j])) {
                count++;
            }
        }
    }
    
    return count;
}

int main(void) {
    char *list[] = {"abc", "def", "cba", "fed", "ghi"};
    int size = sizeof(list) / sizeof(list[0]);
    
    int result = countReversePairs(list, size);
    printf("Number of reverse pairs: %d\n", result);
    
    return 0;
}