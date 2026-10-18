#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void removeLeadingZeros(char *ip, size_t ip_size) {
    if (ip == NULL || ip_size == 0) {
        return;
    }
    
    size_t ip_len = strnlen(ip, ip_size);
    if (ip_len == 0 || ip_len == ip_size) {
        return;
    }

    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char result[16];
    memset(result, 0, sizeof(result));
    size_t result_len = 0;
    size_t result_size = sizeof(result);
    
    char *saveptr = NULL;
    char *token = strtok_r(ip, ".", &saveptr);
    bool first = true;

    while (token != NULL) {
        while (token[0] == '0' && token[1] != '\0') {
            token++;
        }
        
        size_t token_len = strnlen(token, ip_size);
        size_t needed = token_len + (first ? 0 : 1);
        
        if (result_len + needed >= result_size) {
            break;
        }
        
        if (!first) {
            result[result_len++] = '.';
        }
        
        if (result_len + token_len < result_size) {
            size_t remaining = result_size - result_len - 1;
            size_t copy_len = (token_len < remaining) ? token_len : remaining;
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + result_len, token, copy_len);
            result_len += copy_len;
            result[result_len] = '\0';
        }
        
        first = false;
        token = strtok_r(NULL, ".", &saveptr);
    }

    if (result_len > 0 && result_len < ip_size) {
        if (result_len + 1 <= ip_size) {
            size_t copy_size = (result_len + 1 < ip_size) ? result_len + 1 : ip_size;
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(ip, result, copy_size);
            if (copy_size == ip_size) {
                ip[ip_size - 1] = '\0';
            }
        }
    }
}

int main(void) {
    char ip1[] = "192.168.001.001";
    char ip2[] = "010.000.000.001";
    char ip3[] = "000.000.000.000";
    char ip4[] = "255.255.255.255";

    printf("Original: %s\n", ip1);
    removeLeadingZeros(ip1, sizeof(ip1));
    printf("Modified: %s\n\n", ip1);

    printf("Original: %s\n", ip2);
    removeLeadingZeros(ip2, sizeof(ip2));
    printf("Modified: %s\n\n", ip2);

    printf("Original: %s\n", ip3);
    removeLeadingZeros(ip3, sizeof(ip3));
    printf("Modified: %s\n\n", ip3);

    printf("Original: %s\n", ip4);
    removeLeadingZeros(ip4, sizeof(ip4));
    printf("Modified: %s\n", ip4);

    return 0;
}