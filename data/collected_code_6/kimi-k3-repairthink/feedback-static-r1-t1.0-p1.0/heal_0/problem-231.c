#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int maxPathSum(int **triangle, int numRows) {
    if (triangle == NULL || numRows <= 0) {
        return 0;
    }
    
    int *dp = (int *)malloc((size_t)numRows * sizeof(int));
    if (dp == NULL) {
        return 0;
    }
    
    memcpy(dp, triangle[numRows - 1], (size_t)numRows * sizeof(int));
    
    for (int i = numRows - 2; i >= 0; i--) {
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