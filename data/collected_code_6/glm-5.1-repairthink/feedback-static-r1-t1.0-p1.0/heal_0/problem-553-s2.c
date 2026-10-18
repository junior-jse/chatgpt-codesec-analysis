#include <stdio.h>
#include <stdlib.h>

int tuple_to_float(const char *tuple_str, float *result) {
    if (tuple_str == NULL || result == NULL) {
        return -1;
    }

    int a, b;
    if (sscanf(tuple_str, "(%d, %d)", &a, &b) != 2) {
        return -1;
    }

    if (b == 0) {
        return -1;
    }

    *result = (float)a / (float)b;
    return 0;
}

int main() {
    const char *t1 = "(10, 4)";
    const char *t2 = "(5, 0)";
    const char *t3 = "(invalid, 2)";
    const char *t4 = NULL;
    
    float res;

    if (tuple_to_float(t1, &res) == 0) {
        printf("%f\n", res);
    } else {
        printf("Error\n");
    }

    if (tuple_to_float(t2, &res) == 0) {
        printf("%f\n", res);
    } else {
        printf("Error\n");
    }

    if (tuple_to_float(t3, &res) == 0) {
        printf("%f\n", res);
    } else {
        printf("Error\n");
    }

    if (tuple_to_float(t4, &res) == 0) {
        printf("%f\n", res);
    } else {
        printf("Error\n");
    }

    return 0;
}