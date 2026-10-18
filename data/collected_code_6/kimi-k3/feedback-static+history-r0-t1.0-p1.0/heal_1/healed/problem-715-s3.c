#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

typedef struct {
    int *data;
    size_t size;
} Tuple;

Tuple string_to_tuple(const char *str, size_t len) {
    Tuple t = {NULL, 0};
    if (str == NULL) {
        return t;
    }
    
    size_t count = 0;
    const char *p = str;
    const char *end = str + len;
    while (p < end && *p != '\0') {
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
    
    char *copy = malloc(len + 1);
    if (copy == NULL) {
        free(t.data);
        t.data = NULL;
        t.size = 0;
        return t;
    }
    memcpy(copy, str, len);
    copy[len] = '\0';
    
    char *token = strtok(copy, ",");
    size_t i = 0;
    while (token != NULL && i < t.size) {
        char *endptr;
        errno = 0;
        long val = strtol(token, &endptr, 10);
        
        if (errno == ERANGE || val > INT_MAX || val < INT_MIN) {
            free(copy);
            free(t.data);
            t.data = NULL;
            t.size = 0;
            return t;
        }
        
        t.data[i] = (int)val;
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
    Tuple t = string_to_tuple(input, strlen(input));
    
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