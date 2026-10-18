#include <stdio.h>
#include <stdlib.h>

int max_path_sum(int triangle[][10], int rows) {
    int dp[10][10];
    
    for (int j = 0; j < rows; j++) {
        dp[rows - 1][j] = triangle[rows - 1][j];
    }
    
    for (int i = rows - 2; i >= 0; i--) {
        for (int j = 0; j <= i; j++) {
            dp[i][j] = triangle[i][j] + 
                      (dp[i + 1][j] > dp[i + 1][j + 1] ? dp[i + 1][j] : dp[i + 1][j + 1]);
        }
    }
    
    return dp[0][0];
}

int main() {
    int triangle[10][10] = {0};
    int rows;
    
    printf("Enter number of rows: ");
    if (scanf("%d", &rows) != 1 || rows <= 0 || rows > 10) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter triangle numbers:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= i; j++) {
            if (scanf("%d", &triangle[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }
    
    int result = max_path_sum(triangle, rows);
    printf("Maximum path sum: %d\n", result);
    
    return 0;
}