#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
} Tuple;

Tuple string_to_tuple(const char *str) {
    Tuple t = {NULL, 0};
    if (str == NULL) {
        return t;
    }
    
    size_t count = 0;
    const char *p = str;
    while (*p != '\0') {
        if (*p == ',') {
            count++;
        }
        p++;
    }
    if (p != str) {
        count++;
    }
    
    if (count == 0) {
        return t;
    }
    
    t.data = malloc(count * sizeof(int));
    if (t.data == NULL) {
        return t;
    }
    
    t.size = count;
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    char *copy = malloc(strlen(str) + 1);
    if (copy == NULL) {
        free(t.data);
        t.data = NULL;
        t.size = 0;
        return t;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(copy, str);
    
    char *token = strtok(copy, ",");
    size_t i = 0;
    while (token != NULL && i < t.size) {
        /* Possible weaknesses found:
         * Flawfinder atoi: Unless checked, the resulting number can exceed the expected range (CWE-190). If source untrusted, check both minimum and maximum, even if the input had no minus sign (large numbers can roll over into negative number; consider saving to an unsigned value if that is intended). (risk 2, integer)
         */
        t.data[i] = atoi(token);
        i++;
        token = strtok(NULL, ",");
    }
    
    free(copy);
    return t;
}

void free_tuple(Tuple *t) {
    if (t != NULL) {
        free(t->data);
        t->data = NULL;
        t->size = 0;
    }
}

int main(void) {
    const char *input = "1,2,3,4,5";
    Tuple t = string_to_tuple(input);
    
    if (t.data != NULL) {
        printf("Tuple: (");
        for (size_t i = 0; i < t.size; i++) {
            printf("%d", t.data[i]);
            if (i < t.size - 1) {
                printf(", ");
            }
        }
        printf(")\n");
        free_tuple(&t);
    } else {
        printf("Failed to create tuple\n");
    }
    
    return 0;
}