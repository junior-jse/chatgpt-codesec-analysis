#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

void remove_leading_zeroes(char *ip, size_t ip_size) {
    if (ip == NULL || *ip == '\0' || ip_size == 0) {
        return;
    }

    char result[INET_ADDRSTRLEN];
    memset(result, 0, sizeof(result));

    char temp[INET_ADDRSTRLEN];
    memset(temp, 0, sizeof(temp));

    size_t copy_len = (ip_size < sizeof(temp)) ? ip_size : sizeof(temp) - 1;
    if (copy_len > 0) {
        memcpy(temp, ip, copy_len);
        temp[copy_len] = '\0';
    } else {
        temp[0] = '\0';
    }

    char *token;
    char *saveptr;
    int first = 1;
    size_t current_length = 0;

    token = strtok_r(temp, ".", &saveptr);

    while (token != NULL) {
        char *start = token;
        while (*start == '0' && *(start + 1) != '\0') {
            start++;
        }

        if (!first) {
            if (current_length < sizeof(result) - 1) {
                result[current_length] = '.';
                current_length++;
                result[current_length] = '\0';
            } else {
                break;
            }
        }

        size_t token_len = (start[0] != '\0') ? strlen(start) : 0;
        if (current_length + token_len < sizeof(result)) {
            if (token_len > 0) {
                memcpy(result + current_length, start, token_len);
                current_length += token_len;
                result[current_length] = '\0';
            }
        } else {
            break;
        }

        first = 0;
        token = strtok_r(NULL, ".", &saveptr);
    }

    size_t final_len = strlen(result);
    if (final_len >= ip_size) {
        final_len = ip_size > 0 ? ip_size - 1 : 0;
    }
    if (final_len > 0) {
        memcpy(ip, result, final_len);
    }
    ip[final_len] = '\0';
}

int main(void) {
    char ip1[INET_ADDRSTRLEN] = "192.168.001.001";
    char ip2[INET_ADDRSTRLEN] = "010.000.000.001";
    char ip3[INET_ADDRSTRLEN] = "000.000.000.000";
    char ip4[INET_ADDRSTRLEN] = "255.255.255.255";
    char ip5[INET_ADDRSTRLEN] = "001.002.003.004";

    remove_leading_zeroes(ip1, sizeof(ip1));
    remove_leading_zeroes(ip2, sizeof(ip2));
    remove_leading_zeroes(ip3, sizeof(ip3));
    remove_leading_zeroes(ip4, sizeof(ip4));
    remove_leading_zeroes(ip5, sizeof(ip5));

    printf("%s\n", ip1);
    printf("%s\n", ip2);
    printf("%s\n", ip3);
    printf("%s\n", ip4);
    printf("%s\n", ip5);

    return 0;
}