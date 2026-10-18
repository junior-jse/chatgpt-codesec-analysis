/* Repair history (previous round):
 * - Flawfinder strlen was removed from string_to_tuple() and now appears in safe_strdup, trim_whitespace().
 * - Flawfinder strcpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char **items;
    size_t count;
} Tuple;

static char *safe_strdup(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);
    char *copy = malloc(len + 1);
    if (copy != NULL) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(copy, str, len + 1);
    }
    return copy;
}

static size_t safe_strlen(const char *str, size_t max_len) {
    if (str == NULL) {
        return 0;
    }
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

static char *trim_whitespace(char *str) {
    if (str == NULL) {
        return NULL;
    }
    while (isspace((unsigned char)*str)) {
        str++;
    }
    if (*str == '\0') {
        return str;
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);
    char *end = str + len - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
    return str;
}

Tuple string_to_tuple(const char *str) {
    Tuple tuple;
    tuple.items = NULL;
    tuple.count = 0;
    
    if (str == NULL) {
        return tuple;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);
    if (len == 0) {
        return tuple;
    }
    
    char *copy = malloc(len + 1);
    if (copy == NULL) {
        return tuple;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, str, len + 1);
    
    size_t capacity = 8;
    tuple.items = malloc(capacity * sizeof(char *));
    if (tuple.items == NULL) {
        free(copy);
        return tuple;
    }
    
    char *start = copy;
    char *p = copy;
    int in_quotes = 0;
    
    while (*p) {
        if (*p == '"') {
            in_quotes = !in_quotes;
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t remaining = strlen(p + 1);
            memmove(p, p + 1, remaining + 1);
        } else if (*p == ',' && !in_quotes) {
            *p = '\0';
            /* Possible weaknesses found:
             *  Variable 'trimmed' can be declared as pointer to const [constVariablePointer]
             */
            char *trimmed = trim_whitespace(start);
            if (*trimmed != '\0') {
                if (tuple.count >= capacity) {
                    capacity *= 2;
                    char **new_items = realloc(tuple.items, capacity * sizeof(char *));
                    if (new_items == NULL) {
                        free(copy);
                        for (size_t i = 0; i < tuple.count; i++) {
                            free(tuple.items[i]);
                        }
                        free(tuple.items);
                        tuple.items = NULL;
                        tuple.count = 0;
                        return tuple;
                    }
                    tuple.items = new_items;
                }
                tuple.items[tuple.count] = safe_strdup(trimmed);
                if (tuple.items[tuple.count] == NULL) {
                    free(copy);
                    for (size_t i = 0; i < tuple.count; i++) {
                        free(tuple.items[i]);
                    }
                    free(tuple.items);
                    tuple.items = NULL;
                    tuple.count = 0;
                    return tuple;
                }
                tuple.count++;
            }
            start = p + 1;
        }
        p++;
    }
    
    /* Possible weaknesses found:
     *  Variable 'trimmed' can be declared as pointer to const [constVariablePointer]
     */
    char *trimmed = trim_whitespace(start);
    if (*trimmed != '\0') {
        if (tuple.count >= capacity) {
            capacity *= 2;
            char **new_items = realloc(tuple.items, capacity * sizeof(char *));
            if (new_items == NULL) {
                free(copy);
                for (size_t i = 0; i < tuple.count; i++) {
                    free(tuple.items[i]);
                }
                free(tuple.items);
                tuple.items = NULL;
                tuple.count = 0;
                return tuple;
            }
            tuple.items = new_items;
        }
        tuple.items[tuple.count] = safe_strdup(trimmed);
        if (tuple.items[tuple.count] == NULL) {
            free(copy);
            for (size_t i = 0; i < tuple.count; i++) {
                free(tuple.items[i]);
            }
            free(tuple.items);
            tuple.items = NULL;
            tuple.count = 0;
            return tuple;
        }
        tuple.count++;
    }
    
    free(copy);
    return tuple;
}

void free_tuple(Tuple *tuple) {
    if (tuple == NULL) {
        return;
    }
    for (size_t i = 0; i < tuple->count; i++) {
        free(tuple->items[i]);
    }
    free(tuple->items);
    tuple->items = NULL;
    tuple->count = 0;
}

int main(void) {
    const char *test1 = "hello, world, foo, bar";
    const char *test2 = "\"hello, world\", foo, bar";
    const char *test3 = "  spaced  ,  items  ";
    const char *test4 = "";
    const char *test5 = "single";
    
    Tuple t1 = string_to_tuple(test1);
    printf("Test 1: %zu items\n", t1.count);
    for (size_t i = 0; i < t1.count; i++) {
        printf("  [%zu]: %s\n", i, t1.items[i]);
    }
    free_tuple(&t1);
    
    Tuple t2 = string_to_tuple(test2);
    printf("Test 2: %zu items\n", t2.count);
    for (size_t i = 0; i < t2.count; i++) {
        printf("  [%zu]: %s\n", i, t2.items[i]);
    }
    free_tuple(&t2);
    
    Tuple t3 = string_to_tuple(test3);
    printf("Test 3: %zu items\n", t3.count);
    for (size_t i = 0; i < t3.count; i++) {
        printf("  [%zu]: %s\n", i, t3.items[i]);
    }
    free_tuple(&t3);
    
    Tuple t4 = string_to_tuple(test4);
    printf("Test 4: %zu items\n", t4.count);
    free_tuple(&t4);
    
    Tuple t5 = string_to_tuple(test5);
    printf("Test 5: %zu items\n", t5.count);
    for (size_t i = 0; i < t5.count; i++) {
        printf("  [%zu]: %s\n", i, t5.items[i]);
    }
    free_tuple(&t5);
    
    return 0;
}