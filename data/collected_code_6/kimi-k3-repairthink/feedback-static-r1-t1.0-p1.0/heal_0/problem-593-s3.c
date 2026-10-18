#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void removeLeadingZeros(char *ip) {
    if (ip == NULL || strlen(ip) == 0) {
        return;
    }

    char *token;
    char *rest = ip;
    char result[16] = {0};
    int first = 1;

    while ((token = strtok_r(rest, ".", &rest)) != NULL) {
        while (*token == '0' && *(token + 1) != '\0') {
            token++;
        }

        if (!first) {
            strncat(result, ".", sizeof(result) - strlen(result) - 1);
        }
        strncat(result, token, sizeof(result) - strlen(result) - 1);
        first = 0;
    }

    if (strlen(result) > 0) {
        strncpy(ip, result, strlen(result) + 1);
    }
}

int main() {
    char ip1[] = "192.168.001.001";
    char ip2[] = "010.000.000.001";
    char ip3[] = "000.000.000.000";
    char ip4[] = "255.255.255.255";

    printf("Original: %s\n", ip1);
    removeLeadingZeros(ip1);
    printf("Modified: %s\n\n", ip1);

    printf("Original: %s\n", ip2);
    removeLeadingZeros(ip2);
    printf("Modified: %s\n\n", ip2);

    printf("Original: %s\n", ip3);
    removeLeadingZeros(ip3);
    printf("Modified: %s\n\n", ip3);

    printf("Original: %s\n", ip4);
    removeLeadingZeros(ip4);
    printf("Modified: %s\n", ip4);

    return 0;
}