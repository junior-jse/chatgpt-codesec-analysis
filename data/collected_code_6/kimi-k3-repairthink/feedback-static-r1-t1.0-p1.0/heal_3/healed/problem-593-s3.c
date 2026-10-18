#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>

#define MAX_IP_RESULT 16
#define WORK_BUFFER_SIZE 64

void removeLeadingZeros(char *ip, size_t ip_size) {
    if (ip == NULL || ip_size == 0) {
        return;
    }

    size_t ip_len = strnlen(ip, ip_size);
    if (ip_len == 0 || ip_len == ip_size || ip_len >= WORK_BUFFER_SIZE) {
        return;
    }

    char buffer[WORK_BUFFER_SIZE];
    int copied = snprintf(buffer, sizeof(buffer), "%s", ip);
    if (copied < 0 || (size_t)copied >= sizeof(buffer)) {
        return;
    }

    char result[MAX_IP_RESULT] = {0};
    size_t result_len = 0;
    int first = 1;

    char *saveptr = NULL;
    char *token = strtok_r(buffer, ".", &saveptr);

    while (token != NULL) {
        while (token[0] == '0' && token[1] != '\0') {
            token++;
        }

        size_t token_len = strnlen(token, MAX_IP_RESULT);
        size_t separator_len = first ? 0u : 1u;

        if (token_len >= MAX_IP_RESULT ||
            result_len + separator_len + token_len >= sizeof(result)) {
            return;
        }

        int written;
        if (first) {
            written = snprintf(result + result_len, sizeof(result) - result_len,
                               "%s", token);
        } else {
            written = snprintf(result + result_len, sizeof(result) - result_len,
                               ".%s", token);
        }
        if (written < 0 || (size_t)written >= sizeof(result) - result_len) {
            return;
        }

        result_len += (size_t)written;
        first = 0;

        token = strtok_r(NULL, ".", &saveptr);
    }

    if (result_len > 0 && result_len < ip_size) {
        snprintf(ip, ip_size, "%s", result);
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