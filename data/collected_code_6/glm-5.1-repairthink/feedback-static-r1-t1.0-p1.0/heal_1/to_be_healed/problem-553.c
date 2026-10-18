#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
 /* Possible weaknesses found:
  *  include '<ctype.h>' or provide a declaration of 'isspace'
  */

typedef struct {
    char *str;
} Tuple;

int tuple_to_float(const Tuple *t, float *out) {
    if (t == NULL || out == NULL) {
        return -1;
    }
    
    if (t->str == NULL) {
        return -1;
    }
    
    char *endptr;
    errno = 0;
    float val = strtof(t->str, &endptr);
    
    if (errno == ERANGE) {
        return -1;
    }
    
    if (endptr == t->str) {
        return -1;
    }
    
    for (; *endptr != '\0'; endptr++) {
        /* Possible weaknesses found:
         *  call to undeclared library function 'isspace' with type 'int (int)'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
         *  include the header <ctype.h> or explicitly provide a declaration for 'isspace'
         *  implicit declaration of function 'isspace' [-Wimplicit-function-declaration]
         */
        if (!isspace((unsigned char)*endptr)) {
            return -1;
        }
    }
    
    *out = val;
    return 0;
}

int main(void) {
    Tuple t1 = {"123.456"};
    Tuple t2 = {"-78.9e2"};
    Tuple t3 = {"not_a_float"};
    Tuple t4 = {"42abc"};
    Tuple t5 = {NULL};
    /* Possible weaknesses found:
     *  Variable 't6' can be declared as pointer to const [constVariablePointer]
     */
    Tuple *t6 = NULL;
    
    float result;
    
    if (tuple_to_float(&t1, &result) == 0) {
        printf("t1: %f\n", result);
    } else {
        printf("t1: Conversion failed\n");
    }
    
    if (tuple_to_float(&t2, &result) == 0) {
        printf("t2: %f\n", result);
    } else {
        printf("t2: Conversion failed\n");
    }
    
    if (tuple_to_float(&t3, &result) == 0) {
        printf("t3: %f\n", result);
    } else {
        printf("t3: Conversion failed\n");
    }
    
    if (tuple_to_float(&t4, &result) == 0) {
        printf("t4: %f\n", result);
    } else {
        printf("t4: Conversion failed\n");
    }
    
    if (tuple_to_float(&t5, &result) == 0) {
        printf("t5: %f\n", result);
    } else {
        printf("t5: Conversion failed\n");
    }
    
    if (tuple_to_float(t6, &result) == 0) {
        printf("t6: %f\n", result);
    } else {
        printf("t6: Conversion failed\n");
    }
    
    return 0;
}