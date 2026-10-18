#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    void **elements;
    int tuple_size;
} Tuple;

typedef struct {
    Tuple *tuples;
    int count;
} TupleList;

void *extract_nth_element(TupleList *list, int tuple_index, int element_index) {
    if (list == NULL || tuple_index < 0 || tuple_index >= list->count) {
        return NULL;
    }
    
    Tuple *tuple = &list->tuples[tuple_index];
    
    if (tuple->elements == NULL || element_index < 0 || element_index >= tuple->tuple_size) {
        return NULL;
    }
    
    return tuple->elements[element_index];
}

int main(void) {
    int a = 10, b = 20, c = 30;
    int d = 40, e = 50, f = 60;
    /* Possible weaknesses found:
     *  Variable 's1' is assigned a value that is never used. [unreadVariable]
     *  Variable 's1' can be declared as pointer to const [constVariablePointer]
     *  Value stored to 's1' during its initialization is never read [deadcode.DeadStores]
     */
    char *s1 = "hello";
    /* Possible weaknesses found:
     *  Variable 's2' can be declared as pointer to const [constVariablePointer]
     *  Variable 's2' is assigned a value that is never used. [unreadVariable]
     *  Value stored to 's2' during its initialization is never read [deadcode.DeadStores]
     */
    char *s2 = "world";
    
    TupleList list;
    list.count = 2;
    list.tuples = malloc(sizeof(Tuple) * list.count);
    
    if (list.tuples == NULL) {
        return 1;
    }
    
    list.tuples[0].tuple_size = 3;
    list.tuples[0].elements = malloc(sizeof(void *) * 3);
    if (list.tuples[0].elements == NULL) {
        free(list.tuples);
        return 1;
    }
    list.tuples[0].elements[0] = &a;
    list.tuples[0].elements[1] = &b;
    list.tuples[0].elements[2] = &c;
    
    list.tuples[1].tuple_size = 3;
    list.tuples[1].elements = malloc(sizeof(void *) * 3);
    if (list.tuples[1].elements == NULL) {
        free(list.tuples[0].elements);
        free(list.tuples);
        return 1;
    }
    list.tuples[1].elements[0] = &d;
    list.tuples[1].elements[1] = &e;
    list.tuples[1].elements[2] = &f;
    
    /* Possible weaknesses found:
     *  Variable 'result1' can be declared as pointer to const [constVariablePointer]
     */
    int *result1 = (int *)extract_nth_element(&list, 0, 1);
    if (result1 != NULL) {
        printf("%d\n", *result1);
    }
    
    /* Possible weaknesses found:
     *  Variable 'result2' can be declared as pointer to const [constVariablePointer]
     */
    int *result2 = (int *)extract_nth_element(&list, 1, 2);
    if (result2 != NULL) {
        printf("%d\n", *result2);
    }
    
    /* Possible weaknesses found:
     *  Variable 'result3' can be declared as pointer to const [constVariablePointer]
     */
    void *result3 = extract_nth_element(&list, 0, 5);
    if (result3 == NULL) {
        printf("Invalid index\n");
    }
    
    free(list.tuples[0].elements);
    free(list.tuples[1].elements);
    free(list.tuples);
    
    return 0;
}