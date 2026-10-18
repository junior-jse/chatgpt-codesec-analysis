#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    size_t length;
    /* Possible weaknesses found:
     *  struct member 'Tuple::elements' is never used. [unusedStructMember]
     */
    int *elements;
} Tuple;

bool all_tuples_equal_length(const Tuple *tuples, size_t tuple_count) {
    if (tuples == NULL && tuple_count > 0) {
        return false;
    }
    
    if (tuple_count == 0) {
        return true;
    }
    
    size_t first_length = tuples[0].length;
    
    for (size_t i = 1; i < tuple_count; i++) {
        if (tuples[i].length != first_length) {
            return false;
        }
    }
    
    return true;
}

int main(void) {
    int tuple1_elements[] = {1, 2, 3};
    int tuple2_elements[] = {4, 5, 6};
    int tuple3_elements[] = {7, 8, 9};
    int tuple4_elements[] = {10, 11};
    
    const Tuple tuples[] = {
        {3, tuple1_elements},
        {3, tuple2_elements},
        {3, tuple3_elements}
    };
    
    bool result1 = all_tuples_equal_length(tuples, 3);
    printf("All tuples equal length (test 1): %s\n", result1 ? "true" : "false");
    
    const Tuple tuples2[] = {
        {3, tuple1_elements},
        {3, tuple2_elements},
        {2, tuple4_elements}
    };
    
    bool result2 = all_tuples_equal_length(tuples2, 3);
    printf("All tuples equal length (test 2): %s\n", result2 ? "true" : "false");
    
    /* Possible weaknesses found:
     *  Assignment 'result3=all_tuples_equal_length(NULL,0)', assigned value is 1
     *  Calling function 'all_tuples_equal_length' returns 1
     */
    bool result3 = all_tuples_equal_length(NULL, 0);
    /* Possible weaknesses found:
     *  Condition 'result3' is always true
     *  Condition 'result3' is always true [knownConditionTrueFalse]
     */
    printf("All tuples equal length (empty): %s\n", result3 ? "true" : "false");
    
    bool result4 = all_tuples_equal_length(tuples, 0);
    printf("All tuples equal length (zero count): %s\n", result4 ? "true" : "false");
    
    printf("%d %d\n", (int)result3, (int)result4);
    
    return 0;
}