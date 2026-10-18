#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Tuple {
    int initial;
    char *data;
    struct Tuple *next;
} Tuple;

Tuple *join_tuples(Tuple *head) {
    if (head == NULL) {
        return NULL;
    }
    Tuple *result = NULL;
    Tuple *result_tail = NULL;
    Tuple *current = head;
    while (current != NULL) {
        Tuple *existing = result;
        while (existing != NULL) {
            if (existing->initial == current->initial) {
                /* Possible weaknesses found:
                 * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                 */
                size_t len = strlen(existing->data) + strlen(current->data) + 1;
                char *new_data = calloc(len, sizeof(char));
                if (new_data == NULL) {
                    return NULL;
                }
                /* Possible weaknesses found:
                 * Flawfinder strcat: Does not check for buffer overflows when concatenating to destination [MS-banned] (CWE-120). Consider using strcat_s, strncat, strlcat, or snprintf (warning: strncat is easily misused). (risk 4, buffer)
                 */
                strcat(new_data, existing->data);
                /* Possible weaknesses found:
                 * Flawfinder strcat: Does not check for buffer overflows when concatenating to destination [MS-banned] (CWE-120). Consider using strcat_s, strncat, strlcat, or snprintf (warning: strncat is easily misused). (risk 4, buffer)
                 */
                strcat(new_data, current->data);
                free(existing->data);
                existing->data = new_data;
                break;
            }
            existing = existing->next;
        }
        if (existing == NULL) {
            Tuple *new_tuple = malloc(sizeof(Tuple));
            if (new_tuple == NULL) {
                return NULL;
            }
            new_tuple->initial = current->initial;
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            new_tuple->data = malloc(strlen(current->data) + 1);
            if (new_tuple->data == NULL) {
                free(new_tuple);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
             */
            strcpy(new_tuple->data, current->data);
            new_tuple->next = NULL;
            if (result == NULL) {
                result = new_tuple;
                result_tail = new_tuple;
            } else {
                result_tail->next = new_tuple;
                result_tail = new_tuple;
            }
        }
        current = current->next;
    }
    return result;
}

void free_tuple_list(Tuple *head) {
    Tuple *current = head;
    while (current != NULL) {
        Tuple *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

int main(void) {
    Tuple *head = NULL;
    Tuple *tail = NULL;
    int initials[] = {1, 2, 1, 3, 2};
    /* Possible weaknesses found:
     *  Variable 'datas' can be declared as const array [constVariable]
     */
    char *datas[] = {"a", "b", "c", "d", "e"};
    size_t count = sizeof(initials) / sizeof(initials[0]);
    for (size_t i = 0; i < count; i++) {
        Tuple *new_tuple = malloc(sizeof(Tuple));
        if (new_tuple == NULL) {
            free_tuple_list(head);
            return 1;
        }
        new_tuple->initial = initials[i];
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        new_tuple->data = malloc(strlen(datas[i]) + 1);
        if (new_tuple->data == NULL) {
            free(new_tuple);
            free_tuple_list(head);
            return 1;
        }
        /* Possible weaknesses found:
         * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
         */
        strcpy(new_tuple->data, datas[i]);
        new_tuple->next = NULL;
        if (head == NULL) {
            head = new_tuple;
            tail = new_tuple;
        } else {
            tail->next = new_tuple;
            tail = new_tuple;
        }
    }
    Tuple *joined = join_tuples(head);
    Tuple *current = joined;
    while (current != NULL) {
        printf("%d: %s\n", current->initial, current->data);
        current = current->next;
    }
    free_tuple_list(head);
    free_tuple_list(joined);
    return 0;
}