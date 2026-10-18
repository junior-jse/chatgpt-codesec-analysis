#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *elements;
    int size;
} Tuple;

typedef struct {
    Tuple *tuples;
    int count;
    int capacity;
} TupleList;

int initTupleList(TupleList *list, int capacity) {
    if (list == NULL || capacity <= 0) {
        return 0;
    }
    list->tuples = (Tuple *)malloc(sizeof(Tuple) * capacity);
    if (list->tuples == NULL) {
        return 0;
    }
    list->count = 0;
    list->capacity = capacity;
    return 1;
}

void freeTupleList(TupleList *list) {
    if (list == NULL || list->tuples == NULL) {
        return;
    }
    for (int i = 0; i < list->count; i++) {
        free(list->tuples[i].elements);
    }
    free(list->tuples);
    list->tuples = NULL;
    list->count = 0;
    list->capacity = 0;
}

int addTuple(TupleList *list, int *elements, int size) {
    if (list == NULL || elements == NULL || size <= 0) {
        return 0;
    }
    if (list->count >= list->capacity) {
        int newCapacity = list->capacity * 2;
        Tuple *newTuples = (Tuple *)realloc(list->tuples, sizeof(Tuple) * newCapacity);
        if (newTuples == NULL) {
            return 0;
        }
        list->tuples = newTuples;
        list->capacity = newCapacity;
    }
    list->tuples[list->count].elements = (int *)malloc(sizeof(int) * size);
    if (list->tuples[list->count].elements == NULL) {
        return 0;
    }
    memcpy(list->tuples[list->count].elements, elements, sizeof(int) * size);
    list->tuples[list->count].size = size;
    list->count++;
    return 1;
}

int joinSimilarTuples(TupleList *input, TupleList *output) {
    if (input == NULL || output == NULL || input->count == 0) {
        return 0;
    }
    int *visited = (int *)calloc(input->count, sizeof(int));
    if (visited == NULL) {
        return 0;
    }
    for (int i = 0; i < input->count; i++) {
        if (visited[i]) {
            continue;
        }
        int totalSize = 0;
        for (int j = i; j < input->count; j++) {
            if (!visited[j] && input->tuples[j].elements[0] == input->tuples[i].elements[0]) {
                totalSize += input->tuples[j].size;
                visited[j] = 1;
            }
        }
        int *joined = (int *)malloc(sizeof(int) * totalSize);
        if (joined == NULL) {
            free(visited);
            return 0;
        }
        int offset = 0;
        for (int j = i; j < input->count; j++) {
            if (visited[j] && input->tuples[j].elements[0] == input->tuples[i].elements[0]) {
                memcpy(joined + offset, input->tuples[j].elements, sizeof(int) * input->tuples[j].size);
                offset += input->tuples[j].size;
            }
        }
        if (!addTuple(output, joined, totalSize)) {
            free(joined);
            free(visited);
            return 0;
        }
        free(joined);
    }
    free(visited);
    return 1;
}

void printTuples(TupleList *list) {
    if (list == NULL) {
        return;
    }
    for (int i = 0; i < list->count; i++) {
        printf("(");
        for (int j = 0; j < list->tuples[i].size; j++) {
            printf("%d", list->tuples[i].elements[j]);
            if (j < list->tuples[i].size - 1) {
                printf(", ");
            }
        }
        printf(")\n");
    }
}

int main(void) {
    TupleList input, output;
    if (!initTupleList(&input, 4)) {
        return 1;
    }
    if (!initTupleList(&output, 4)) {
        freeTupleList(&input);
        return 1;
    }
    int t1[] = {1, 2};
    int t2[] = {3, 4};
    int t3[] = {1, 5};
    int t4[] = {3, 6, 7};
    int t5[] = {2, 8};
    if (!addTuple(&input, t1, 2) || !addTuple(&input, t2, 2) ||
        !addTuple(&input, t3, 2) || !addTuple(&input, t4, 3) ||
        !addTuple(&input, t5, 2)) {
        freeTupleList(&input);
        freeTupleList(&output);
        return 1;
    }
    printf("Input tuples:\n");
    printTuples(&input);
    if (!joinSimilarTuples(&input, &output)) {
        freeTupleList(&input);
        freeTupleList(&output);
        return 1;
    }
    printf("\nJoined tuples:\n");
    printTuples(&output);
    freeTupleList(&input);
    freeTupleList(&output);
    return 0;
}