#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct KeyValue {
    char *key;
    int value;
    struct KeyValue *next;
} KeyValue;

typedef struct {
    KeyValue *head;
} Dictionary;

void dict_init(Dictionary *dict) {
    dict->head = NULL;
}

int dict_add(Dictionary *dict, const char *key, int value) {
    KeyValue *current = dict->head;
    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value += value;
            return 0;
        }
        current = current->next;
    }

    KeyValue *new_pair = (KeyValue *)malloc(sizeof(KeyValue));
    if (!new_pair) {
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    new_pair->key = (char *)malloc(strlen(key) + 1);
    if (!new_pair->key) {
        free(new_pair);
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(new_pair->key, key);
    new_pair->value = value;
    new_pair->next = dict->head;
    dict->head = new_pair;
    return 0;
}

Dictionary dict_combine(Dictionary *dict1, Dictionary *dict2) {
    Dictionary result;
    dict_init(&result);

    KeyValue *current = dict1->head;
    while (current) {
        if (dict_add(&result, current->key, current->value) != 0) {
            /* Possible weaknesses found:
             *  call to undeclared function 'dict_free'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
             *  implicit declaration of function 'dict_free' [-Wimplicit-function-declaration]
             *  previous implicit declaration is here
             */
            dict_free(&result);
            dict_init(&result);
            return result;
        }
        current = current->next;
    }

    current = dict2->head;
    while (current) {
        if (dict_add(&result, current->key, current->value) != 0) {
            /* Possible weaknesses found:
             *  call to undeclared function 'dict_free'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
             */
            dict_free(&result);
            dict_init(&result);
            return result;
        }
        current = current->next;
    }

    return result;
}

/* Possible weaknesses found:
 *  conflicting types for 'dict_free'
 */
void dict_free(Dictionary *dict) {
    KeyValue *current = dict->head;
    while (current) {
        KeyValue *temp = current;
        current = current->next;
        free(temp->key);
        free(temp);
    }
    dict->head = NULL;
}

void dict_print(Dictionary *dict) {
    KeyValue *current = dict->head;
    while (current) {
        printf("%s: %d\n", current->key, current->value);
        current = current->next;
    }
}

int main(void) {
    Dictionary dict1, dict2, combined;
    dict_init(&dict1);
    dict_init(&dict2);
    dict_init(&combined);

    dict_add(&dict1, "apple", 1);
    dict_add(&dict1, "banana", 2);
    dict_add(&dict2, "banana", 3);
    dict_add(&dict2, "cherry", 4);

    combined = dict_combine(&dict1, &dict2);
    dict_print(&combined);

    dict_free(&dict1);
    dict_free(&dict2);
    dict_free(&combined);

    return 0;
}