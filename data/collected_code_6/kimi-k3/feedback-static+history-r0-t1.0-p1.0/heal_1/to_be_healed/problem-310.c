#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} Tuple;

Tuple *tuple_create(void) {
    Tuple *t = malloc(sizeof(Tuple));
    if (t == NULL) {
        return NULL;
    }
    t->capacity = 8;
    t->count = 0;
    t->items = malloc(t->capacity * sizeof(char *));
    if (t->items == NULL) {
        free(t);
        return NULL;
    }
    return t;
}

int tuple_append(Tuple *t, const char *item) {
    if (t == NULL || item == NULL) {
        return -1;
    }
    if (t->count >= t->capacity) {
        size_t new_cap = t->capacity * 2;
        char **new_items = realloc(t->items, new_cap * sizeof(char *));
        if (new_items == NULL) {
            return -1;
        }
        t->items = new_items;
        t->capacity = new_cap;
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    t->items[t->count] = malloc(strlen(item) + 1);
    if (t->items[t->count] == NULL) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(t->items[t->count], item);
    t->count++;
    return 0;
}

void tuple_free(Tuple *t) {
    if (t == NULL) {
        return;
    }
    for (size_t i = 0; i < t->count; i++) {
        free(t->items[i]);
    }
    free(t->items);
    free(t);
}

Tuple *string_to_tuple(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    
    Tuple *t = tuple_create();
    if (t == NULL) {
        return NULL;
    }
    
    const char *start = str;
    const char *end = str;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[256];
    /* Possible weaknesses found:
     *  Variable 'buf_idx' is assigned a value that is never used. [unreadVariable]
     */
    size_t buf_idx = 0;
    int in_quotes = 0;
    
    while (*end != '\0') {
        if (*end == '"') {
            in_quotes = !in_quotes;
        } else if (*end == ',' && !in_quotes) {
            size_t len = (size_t)(end - start);
            if (len >= sizeof(buffer)) {
                tuple_free(t);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(buffer, start, len);
            buffer[len] = '\0';
            
            char *trimmed = buffer;
            while (isspace((unsigned char)*trimmed)) {
                trimmed++;
            }
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            char *trim_end = trimmed + strlen(trimmed);
            while (trim_end > trimmed && isspace((unsigned char)trim_end[-1])) {
                trim_end--;
            }
            *trim_end = '\0';
            
            if (tuple_append(t, trimmed) != 0) {
                tuple_free(t);
                return NULL;
            }
            start = end + 1;
        }
        end++;
    }
    
    size_t len = (size_t)(end - start);
    if (len >= sizeof(buffer)) {
        tuple_free(t);
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(buffer, start, len);
    buffer[len] = '\0';
    
    char *trimmed = buffer;
    while (isspace((unsigned char)*trimmed)) {
        trimmed++;
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    char *trim_end = trimmed + strlen(trimmed);
    while (trim_end > trimmed && isspace((unsigned char)trim_end[-1])) {
        trim_end--;
    }
    *trim_end = '\0';
    
    if (tuple_append(t, trimmed) != 0) {
        tuple_free(t);
        return NULL;
    }
    
    return t;
}

int main(void) {
    const char *input = "apple, banana, \"cherry, red\", date";
    Tuple *t = string_to_tuple(input);
    
    if (t == NULL) {
        fprintf(stderr, "Failed to convert string to tuple\n");
        return EXIT_FAILURE;
    }
    
    printf("(");
    for (size_t i = 0; i < t->count; i++) {
        printf("'%s'", t->items[i]);
        if (i < t->count - 1) {
            printf(", ");
        }
    }
    printf(")\n");
    
    tuple_free(t);
    return EXIT_SUCCESS;
}