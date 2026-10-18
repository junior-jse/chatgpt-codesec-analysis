#include <stdio.h>
#include <stdbool.h>
#include <string.h>
 /* Possible weaknesses found:
  *  include '<stdlib.h>' or provide a declaration of 'malloc'
  */

#define TABLE_SIZE 100

typedef struct Entry {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char key[50];
    int value;
    struct Entry *next;
} Entry;

typedef struct Dictionary {
    Entry *buckets[TABLE_SIZE];
} Dictionary;

unsigned int hash(const char *key) {
    unsigned int hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

Dictionary *create_dictionary(void) {
    /* Possible weaknesses found:
     *  call to undeclared library function 'malloc' with type 'void *(unsigned long)'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
     *  implicit declaration of function 'malloc' [-Wimplicit-function-declaration]
     *  include the header <stdlib.h> or explicitly provide a declaration for 'malloc'
     */
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        dict->buckets[i] = NULL;
    }
    return dict;
}

bool dictionary_contains(Dictionary *dict, const char *key) {
    if (dict == NULL || key == NULL) {
        return false;
    }
    
    unsigned int index = hash(key);
    Entry *current = dict->buckets[index];
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return true;
        }
        current = current->next;
    }
    
    return false;
}

bool dictionary_insert(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL) {
        return false;
    }
    
    if (dictionary_contains(dict, key)) {
        return false;
    }
    
    unsigned int index = hash(key);
    Entry *new_entry = (Entry *)malloc(sizeof(Entry));
    if (new_entry == NULL) {
        return false;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(new_entry->key, key, sizeof(new_entry->key) - 1);
    new_entry->key[sizeof(new_entry->key) - 1] = '\0';
    new_entry->value = value;
    new_entry->next = dict->buckets[index];
    dict->buckets[index] = new_entry;
    
    return true;
}

void destroy_dictionary(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *current = dict->buckets[i];
        while (current != NULL) {
            Entry *next = current->next;
            /* Possible weaknesses found:
             *  include '<stdlib.h>' or provide a declaration of 'free'
             *  include the header <stdlib.h> or explicitly provide a declaration for 'free'
             *  implicit declaration of function 'free' [-Wimplicit-function-declaration]
             *  call to undeclared library function 'free' with type 'void (void *)'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
             */
            free(current);
            current = next;
        }
    }
    
    free(dict);
}

int main(void) {
    Dictionary *dict = create_dictionary();
    if (dict == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return 1;
    }
    
    dictionary_insert(dict, "apple", 1);
    dictionary_insert(dict, "banana", 2);
    dictionary_insert(dict, "cherry", 3);
    dictionary_insert(dict, "date", 4);
    
    const char *keys_to_check[] = {"apple", "banana", "grape", "date", "fig"};
    size_t num_keys = sizeof(keys_to_check) / sizeof(keys_to_check[0]);
    
    for (size_t i = 0; i < num_keys; i++) {
        if (dictionary_contains(dict, keys_to_check[i])) {
            printf("Key '%s' is present in the dictionary.\n", keys_to_check[i]);
        } else {
            printf("Key '%s' is NOT present in the dictionary.\n", keys_to_check[i]);
        }
    }
    
    destroy_dictionary(dict);
    
    return 0;
}