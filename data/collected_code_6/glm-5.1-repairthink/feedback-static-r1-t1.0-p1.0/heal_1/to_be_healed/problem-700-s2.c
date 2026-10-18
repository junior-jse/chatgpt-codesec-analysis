#include <stdio.h>
#include <stdlib.h>

/* Possible weaknesses found:
 *  Parameter 'list' can be declared as pointer to const [constParameterPointer]
 */
int count_in_range(int *list, size_t size, int lower, int upper) {
    if (list == NULL) {
        return 0;
    }

    int count = 0;
    for (size_t i = 0; i < size; i++) {
        if (list[i] >= lower && list[i] <= upper) {
            count++;
        }
    }
    return count;
}

int main(void) {
    int data[] = {1, 5, 12, 15, 20, 25, 30};
    size_t data_size = sizeof(data) / sizeof(data[0]);
    
    int lower_bound = 10;
    int upper_bound = 25;
    
    int result = count_in_range(data, data_size, lower_bound, upper_bound);
    printf("%d\n", result);
    
    return 0;
}