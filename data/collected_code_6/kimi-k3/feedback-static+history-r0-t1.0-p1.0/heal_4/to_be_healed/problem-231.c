#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int maxPathSum(int **triangle, int numRows) {
    if (triangle == NULL || numRows <= 0) {
        return 0;
    }
    
    if ((size_t)numRows > SIZE_MAX / sizeof(int)) {
        return 0;
    }
    
    int *dp = (int *)malloc((size_t)numRows * sizeof(int));
    if (dp == NULL) {
        return 0;
    }
    
    if (triangle[numRows - 1] == NULL) {
        free(dp);
        return 0;
    }
    
    /* Possible weaknesses found:
     *  'copySize' is assigned value '(unsigned long)numRows*sizeof(int)' here.
     */
    size_t copySize = (size_t)numRows * sizeof(int);
    
    /* Possible weaknesses found:
     *  The comparison 'copySize > (unsigned long)numRows*sizeof(int)' is always false because 'copySize' and '(unsigned long)numRows*sizeof(int)' represent the same value. [knownConditionTrueFalse]
     *  The comparison 'copySize > (unsigned long)numRows*sizeof(int)' is always false because 'copySize' and '(unsigned long)numRows*sizeof(int)' represent the same value.
     */
    if (copySize > (size_t)numRows * sizeof(int)) {
        free(dp);
        return 0;
    }
    
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(dp, triangle[numRows - 1], copySize);
    
    for (int i = numRows - 2; i >= 0; i--) {
        if (triangle[i] == NULL) {
            free(dp);
            return 0;
        }
        for (int j = 0; j <= i; j++) {
            dp[j] = triangle[i][j] + (dp[j] > dp[j + 1] ? dp[j] : dp[j + 1]);
        }
    }
    
    int result = dp[0];
    free(dp);
    dp = NULL;
    
    return result;
}

int main(void) {
    int row0[] = {3};
    int row1[] = {7, 4};
    int row2[] = {2, 4, 6};
    int row3[] = {8, 5, 9, 3};
    
    int *triangle[] = {row0, row1, row2, row3};
    int numRows = 4;
    
    int result = maxPathSum(triangle, numRows);
    
    printf("Maximum path sum: %d\n", result);
    
    return 0;
}