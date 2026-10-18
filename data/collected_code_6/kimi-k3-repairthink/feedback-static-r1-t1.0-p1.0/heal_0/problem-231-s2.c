#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_ROWS 100

int find_max_path_sum(int triangle[MAX_ROWS][MAX_ROWS], int rows) {
    int dp[MAX_ROWS] = {0};
    int i, j;
    
    if (rows <= 0 || rows > MAX_ROWS) {
        return -1;
    }
    
    for (j = 0; j < rows; j++) {
        dp[j] = triangle[rows - 1][j];
    }
    
    for (i = rows - 2; i >= 0; i--) {
        for (j = 0; j <= i; j++) {
            int left = dp[j];
            int right = dp[j + 1];
            int max = (left > right) ? left : right;
            
            if (triangle[i][j] > INT_MAX - max) {
                return -1;
            }
            dp[j] = triangle[i][j] + max;
        }
    }
    
    return dp[0];
}

int main(void) {
    int triangle[MAX_ROWS][MAX_ROWS];
    int rows;
    int i, j;
    int result;
    
    if (scanf("%d", &rows) != 1) {
        return EXIT_FAILURE;
    }
    
    if (rows <= 0 || rows > MAX_ROWS) {
        return EXIT_FAILURE;
    }
    
    for (i = 0; i < rows; i++) {
        for (j = 0; j <= i; j++) {
            if (scanf("%d", &triangle[i][j]) != 1) {
                return EXIT_FAILURE;
            }
        }
    }
    
    result = find_max_path_sum(triangle, rows);
    
    if (result == -1) {
        return EXIT_FAILURE;
    }
    
    printf("%d\n", result);
    
    return EXIT_SUCCESS;
}