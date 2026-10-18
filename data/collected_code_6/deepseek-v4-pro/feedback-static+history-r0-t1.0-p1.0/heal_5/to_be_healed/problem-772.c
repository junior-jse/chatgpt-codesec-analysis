#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* remove_words_of_length(char* str, int k) {
    if (str == NULL || k <= 0) {
        return str;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t max_len = strlen(str);

    char* result = (char*)malloc(max_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0, j = 0;
    
    /* Possible weaknesses found:
     *  Condition 'str[i]!='\0'' is always true [knownConditionTrueFalse]
     */
    while (i < max_len && str[i] != '\0') {
        /* Possible weaknesses found:
         *  Condition 'str[i]!='\0'' is always true [knownConditionTrueFalse]
         */
        while (i < max_len && str[i] != '\0' && isspace((unsigned char)str[i])) {
            result[j++] = str[i];
            i++;
        }

        size_t start = i;
        /* Possible weaknesses found:
         *  Condition 'str[i]!='\0'' is always true [knownConditionTrueFalse]
         */
        while (i < max_len && str[i] != '\0' && !isspace((unsigned char)str[i])) {
            i++;
        }
        size_t word_len = i - start;

        if (word_len != (size_t)k) {
            size_t w;
            for (w = start; w < i; w++) {
                result[j++] = str[w];
            }
        }
    }

    result[j] = '\0';

    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(str, result);
    
    free(result);
    return str;
}

int main(void) {
    char* str = NULL;
    size_t buf_size = 256;
    int k;

    str = (char*)malloc(buf_size);
    if (str == NULL) {
        return 1;
    }

    printf("Enter a string: ");
    if (fgets(str, buf_size, stdin) == NULL) {
        free(str);
        return 1;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--;
    }

    while (len > 0 && str[len - 1] != '\n' && !feof(stdin)) {
        buf_size *= 2;
        char* temp = (char*)realloc(str, buf_size);
        if (temp == NULL) {
            free(str);
            return 1;
        }
        str = temp;
        
        if (fgets(str + len, buf_size - len, stdin) == NULL) {
            break;
        }
        
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        len = strlen(str);
        if (len > 0 && str[len - 1] == '\n') {
            str[len - 1] = '\0';
            /* Possible weaknesses found:
             *  Variable 'len' is assigned a value that is never used. [unreadVariable]
             */
            len--;
            break;
        }
    }

    printf("Enter length k: ");
    if (scanf("%d", &k) != 1) {
        free(str);
        return 1;
    }

    remove_words_of_length(str, k);
    printf("Result: \"%s\"\n", str);

    free(str);
    return 0;
}