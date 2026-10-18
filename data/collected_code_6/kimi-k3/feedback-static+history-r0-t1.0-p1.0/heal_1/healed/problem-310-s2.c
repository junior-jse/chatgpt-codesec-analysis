#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char **items;
    size_t count;
} Tuple;

static void free_tuple(Tuple *tuple) {
    if (tuple == NULL) {
        return;
    }
    if (tuple->items != NULL) {
        for (size_t i = 0; i < tuple->count; i++) {
            free(tuple->items[i]);
        }
        free(tuple->items);
    }
    tuple->items = NULL;
    tuple->count = 0;
}

static char *trim_whitespace(char *str) {
    char *end;
    
    while (isspace((unsigned char)*str)) {
        str++;
    }
    
    if (*str == '\0') {
        return str;
    }
    
    end = str;
    while (*end != '\0') {
        end++;
    }
    end--;
    
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    end[1] = '\0';
    
    return str;
}

static char *safe_strdup(const char *src) {
    size_t len = 0;
    char *dst;
    
    if (src == NULL) {
        return NULL;
    }
    
    while (src[len] != '\0') {
        len++;
    }
    
    dst = malloc(len + 1);
    if (dst == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i <= len; i++) {
        dst[i] = src[i];
    }
    
    return dst;
}

int string_to_tuple(const char *input, Tuple *result) {
    char *buffer;
    char *token;
    char *saveptr;
    char **temp_items;
    size_t capacity = 10;
    size_t count = 0;
    size_t input_len = 0;
    
    if (input == NULL || result == NULL) {
        return -1;
    }
    
    result->items = NULL;
    result->count = 0;
    
    while (input[input_len] != '\0') {
        input_len++;
    }
    
    buffer = malloc(input_len + 1);
    if (buffer == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        buffer[i] = input[i];
    }
    
    temp_items = malloc(capacity * sizeof(char *));
    if (temp_items == NULL) {
        free(buffer);
        return -1;
    }
    
    token = strtok_r(buffer, ",", &saveptr);
    while (token != NULL) {
        char *trimmed = trim_whitespace(token);
        
        if (count >= capacity) {
            char **new_items;
            capacity *= 2;
            new_items = realloc(temp_items, capacity * sizeof(char *));
            if (new_items == NULL) {
                for (size_t i = 0; i < count; i++) {
                    free(temp_items[i]);
                }
                free(temp_items);
                free(buffer);
                return -1;
            }
            temp_items = new_items;
        }
        
        temp_items[count] = safe_strdup(trimmed);
        if (temp_items[count] == NULL) {
            for (size_t i = 0; i < count; i++) {
                free(temp_items[i]);
            }
            free(temp_items);
            free(buffer);
            return -1;
        }
        count++;
        
        token = strtok_r(NULL, ",", &saveptr);
    }
    
    free(buffer);
    
    if (count == 0) {
        free(temp_items);
        result->items = NULL;
        result->count = 0;
        return 0;
    }
    
    result->items = temp_items;
    result->count = count;
    return 0;
}

int main(void) {
    const char *input = "apple, banana, cherry, date";
    Tuple tuple;
    int status;
    
    status = string_to_tuple(input, &tuple);
    if (status != 0) {
        fprintf(stderr, "Error converting string to tuple\n");
        return EXIT_FAILURE;
    }
    
    printf("(");
    for (size_t i = 0; i < tuple.count; i++) {
        printf("\"%s\"", tuple.items[i]);
        if (i < tuple.count - 1) {
            printf(", ");
        }
    }
    printf(")\n");
    
    free_tuple(&tuple);
    
    return EXIT_SUCCESS;
}