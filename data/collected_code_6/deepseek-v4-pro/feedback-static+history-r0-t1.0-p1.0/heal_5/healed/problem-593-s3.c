#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void remove_leading_zeroes(char *ip) {
    if (ip == NULL || *ip == '\0') {
        return;
    }

    size_t ip_len = strnlen(ip, 16);
    if (ip_len > 15) {
        ip[15] = '\0';
        ip_len = 15;
    }

    char *copy = malloc(ip_len + 1);
    if (copy == NULL) {
        return;
    }
    memcpy(copy, ip, ip_len);
    copy[ip_len] = '\0';

    char result[16] = {0};
    char *token;
    char *saveptr;
    size_t result_len = 0;

    token = strtok_r(copy, ".", &saveptr);
    int first = 1;

    while (token != NULL && result_len < 15) {
        char *start = token;
        while (*start == '0' && *(start + 1) != '\0') {
            start++;
        }

        if (!first) {
            if (result_len < 15) {
                result[result_len] = '.';
                result_len++;
            }
        }

        size_t token_len = strnlen(start, 4);
        size_t space_left = 15 - result_len;
        size_t bytes_to_copy = (token_len < space_left) ? token_len : space_left;

        if (bytes_to_copy > 0 && result_len + bytes_to_copy <= 15) {
            memcpy(result + result_len, start, bytes_to_copy);
            result_len += bytes_to_copy;
        }

        first = 0;
        token = strtok_r(NULL, ".", &saveptr);
    }

    result[result_len] = '\0';
    
    size_t final_len = strnlen(result, 15);
    if (final_len <= 15 && final_len <= ip_len) {
        memcpy(ip, result, final_len + 1);
    }
    ip[15] = '\0';

    free(copy);
}

int main(void) {
    char ip1[16] = "192.168.001.001";
    char ip2[16] = "010.000.000.001";
    char ip3[16] = "000.000.000.000";
    char ip4[16] = "255.255.255.255";
    
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