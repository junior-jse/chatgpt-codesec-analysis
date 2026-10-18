#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int *values;
    size_t length;
} Tuple;

static int parse_integer(const char *str, size_t *index) {
    while (str[*index] && isspace((unsigned char)str[*index])) {
        (*index)++;
    }
    
    char *endptr;
    long val = strtol(str + *index, &endptr, 10);
    
    if (endptr == str + *index) {
        return 0;
    }
    
    *index = endptr - str;
    if (*index < strlen(str) && str[*index] == ',') {
        (*index)++;
    }
    
    return (int)val;
}

static size_t count_integers(const char *str) {
    size_t count = 0;
    size_t i = 0;
    int in_number = 0;
    
    while (str[i]) {
        if (isdigit((unsigned char)str[i]) || str[i] == '-') {
            if (!in_number) {
                count++;
                in_number = 1;
            }
        } else if (str[i] == ',' || isspace((unsigned char)str[i])) {
            in_number = 0;
        }
        i++;
    }
    
    return count;
}

static Tuple string_to_tuple(const char *str) {
    Tuple result = {NULL, 0};
    
    if (str == NULL) {
        return result;
    }
    
    size_t count = count_integers(str);
    if (count == 0) {
        return result;
    }
    
    result.values = malloc(count * sizeof(int));
    if (result.values == NULL) {
        return result;
    }
    
    size_t index = 0;
    for (size_t i = 0; i < count; i++) {
        result.values[i] = parse_integer(str, &index);
        result.length++;
    }
    
    return result;
}

static void free_tuple(Tuple *tuple) {
    if (tuple != NULL && tuple->values != NULL) {
        free(tuple->values);
        tuple->values = NULL;
        tuple->length = 0;
    }
}

int main(void) {
    const char *input = "10, 20, 30, 40, 50";
    Tuple result = string_to_tuple(input);
    
    if (result.values == NULL) {
        fprintf(stderr, "Failed to parse tuple\n");
        return EXIT_FAILURE;
    }
    
    printf("Tuple: (");
    for (size_t i = 0; i < result.length; i++) {
        if (i > 0) {
            printf(", ");
        }
        printf("%d", result.values[i]);
    }
    printf(")\n");
    
    free_tuple(&result);
    return EXIT_SUCCESS;
}