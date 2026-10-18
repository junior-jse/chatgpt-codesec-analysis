#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
} Tuple;

int tuples_equal(Tuple t1, Tuple t2) {
    return t1.a == t2.a && t1.b == t2.b;
}

Tuple* remove_matching_tuples(const Tuple* t1, int* s1, const Tuple* t2, int* s2) {
    int i, j, k;
    int match1 = 0, match2 = 0;
    int* flags1 = (int*)calloc(*s1, sizeof(int));
    int* flags2 = (int*)calloc(*s2, sizeof(int));
    
    if (flags1 == NULL || flags2 == NULL) {
        free(flags1);
        free(flags2);
        return NULL;
    }

    for (i = 0; i < *s1; i++) {
        for (j = 0; j < *s2; j++) {
            if (tuples_equal(t1[i], t2[j])) {
                flags1[i] = 1;
                flags2[j] = 1;
                break;
            }
        }
    }

    for (i = 0; i < *s1; i++) {
        if (flags1[i]) match1++;
    }
    for (i = 0; i < *s2; i++) {
        if (flags2[i]) match2++;
    }

    Tuple* result = (Tuple*)malloc((*s1 - match1 + *s2 - match2) * sizeof(Tuple));
    if (result == NULL) {
        free(flags1);
        free(flags2);
        return NULL;
    }

    k = 0;
    for (i = 0; i < *s1; i++) {
        if (!flags1[i]) {
            result[k++] = t1[i];
        }
    }
    for (i = 0; i < *s2; i++) {
        if (!flags2[i]) {
            result[k++] = t2[i];
        }
    }

    *s1 = *s1 - match1;
    *s2 = *s2 - match2;

    free(flags1);
    free(flags2);

    return result;
}

int main() {
    Tuple t1[] = {{1, 2}, {3, 4}, {5, 6}};
    Tuple t2[] = {{3, 4}, {7, 8}, {9, 10}};
    int s1 = sizeof(t1) / sizeof(t1[0]);
    int s2 = sizeof(t2) / sizeof(t2[0]);

    Tuple* res = remove_matching_tuples(t1, &s1, t2, &s2);
    if (res == NULL) {
        return 1;
    }

    int i;
    for (i = 0; i < s1; i++) {
        printf("(%d, %d) ", res[i].a, res[i].b);
    }
    for (i = 0; i < s2; i++) {
        printf("(%d, %d) ", res[s1 + i].a, res[s1 + i].b);
    }
    printf("\n");

    free(res);

    return 0;
}