#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void remove_leading_zeroes(char *ip) {
    if (ip == NULL || *ip == '\0') {
        return;
    }

    char result[16] = {0};
    char *token;
    char *saveptr;
    char *copy = strdup(ip);
    
    if (copy == NULL) {
        return;
    }

    token = strtok_r(copy, ".", &saveptr);
    int first = 1;
    size_t result_len = 0;
    
    while (token != NULL) {
        char *start = token;
        while (*start == '0' && *(start + 1) != '\0') {
            start++;
        }
        
        if (!first) {
            if (result_len < sizeof(result) - 1) {
                result[result_len] = '.';
                result_len++;
                result[result_len] = '\0';
            }
        }
        
        size_t token_len = strlen(start);
        if (result_len + token_len < sizeof(result)) {
            memcpy(result + result_len, start, token_len);
            result_len += token_len;
            result[result_len] = '\0';
        }
        
        first = 0;
        token = strtok_r(NULL, ".", &saveptr);
    }
    
    size_t result_len_final = strlen(result);
    if (result_len_final < 16) {
        memcpy(ip, result, result_len_final + 1);
    } else {
        memcpy(ip, result, 16);
        ip[15] = '\0';
    }
    
    free(copy);
}

int main(void) {
    char ip1[] = "192.168.001.001";
    char ip2[] = "010.000.000.001";
    char ip3[] = "000.000.000.000";
    char ip4[] = "255.255.255.255";
    
    remove_leading_zeroes(ip1);
    remove_leading_zeroes(ip2);
    remove_leading_zeroes(ip3);
    remove_leading_zeroes(ip4);
    
    printf("%s\n", ip1);
    printf("%s\n", ip2);
    printf("%s\n", ip3);
    printf("%s\n", ip4);
    
    return 0;
}