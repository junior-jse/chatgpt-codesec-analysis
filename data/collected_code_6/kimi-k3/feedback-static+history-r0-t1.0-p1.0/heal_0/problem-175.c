#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool is_valid_parentheses(const char *str) {
    if (str == NULL) {
        return false;
    }
    
    int balance = 0;
    size_t len = strlen(str);
    
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '(') {
            balance++;
        } else if (str[i] == ')') {
            balance--;
            if (balance < 0) {
                return false;
            }
        }
    }
    
    return balance == 0;
}

int main(void) {
    const char *test1 = "()";
    const char *test2 = "()()";
    const char *test3 = "(())";
    const char *test4 = "(()";
    const char *test5 = "())";
    const char *test6 = ")(";
    const char *test7 = "";
    const char *test8 = NULL;
    
    printf("Test 1: %s -> %s\n", test1, is_valid_parentheses(test1) ? "Valid" : "Invalid");
    printf("Test 2: %s -> %s\n", test2, is_valid_parentheses(test2) ? "Valid" : "Invalid");
    printf("Test 3: %s -> %s\n", test3, is_valid_parentheses(test3) ? "Valid" : "Invalid");
    printf("Test 4: %s -> %s\n", test4, is_valid_parentheses(test4) ? "Valid" : "Invalid");
    printf("Test 5: %s -> %s\n", test5, is_valid_parentheses(test5) ? "Valid" : "Invalid");
    printf("Test 6: %s -> %s\n", test6, is_valid_parentheses(test6) ? "Valid" : "Invalid");
    printf("Test 7: %s -> %s\n", test7, is_valid_parentheses(test7) ? "Valid" : "Invalid");
    printf("Test 8: NULL -> %s\n", is_valid_parentheses(test8) ? "Valid" : "Invalid");
    
    return 0;
}