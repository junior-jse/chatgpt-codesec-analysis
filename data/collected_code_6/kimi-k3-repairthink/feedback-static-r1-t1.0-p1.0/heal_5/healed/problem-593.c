#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define IP_ADDRESS_BUFFER_SIZE 100
#define MAX_IPV6_ADDRESS_LENGTH 45

_Static_assert(IP_ADDRESS_BUFFER_SIZE > MAX_IPV6_ADDRESS_LENGTH + 1,
               "IP address buffer must be larger than the maximum possible address length");

void removeLeadingZeros(char *ipAddress, size_t ipAddressSize) {
    char *token;
    char *saveptr = NULL;
    char *result;
    size_t resultSize;
    size_t resultLen = 0;
    size_t ipLen;

    if (ipAddress == NULL || ipAddressSize == 0) {
        return;
    }

    ipAddress[ipAddressSize - 1] = '\0';
    ipLen = strnlen(ipAddress, ipAddressSize);

    if (ipLen == 0) {
        return;
    }

    resultSize = ipLen + 1;
    result = (char *)malloc(resultSize);
    if (result == NULL) {
        return;
    }
    result[0] = '\0';

    token = strtok_r(ipAddress, ".", &saveptr);
    while (token != NULL) {
        char *temp = token;
        size_t remaining;
        int written;

        while (*temp == '0') {
            temp++;
        }
        if (*temp == '\0') {
            temp--;
        }

        remaining = resultSize - resultLen;
        if (remaining < 2) {
            break;
        }

        written = snprintf(result + resultLen, remaining, "%s.", temp);
        if (written < 0 || (size_t)written >= remaining) {
            result[resultLen] = '\0';
            break;
        }
        resultLen += (size_t)written;

        token = strtok_r(NULL, ".", &saveptr);
    }

    if (resultLen > 0 && result[resultLen - 1] == '.') {
        result[resultLen - 1] = '\0';
    }

    snprintf(ipAddress, ipAddressSize, "%s", result);
    free(result);
}

int main(void) {
    char *ipAddress;
    size_t inputLen;

    ipAddress = (char *)calloc(IP_ADDRESS_BUFFER_SIZE, sizeof(char));
    if (ipAddress == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    printf("Enter an IP address: ");
    fflush(stdout);

    if (fgets(ipAddress, IP_ADDRESS_BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        free(ipAddress);
        return 1;
    }

    ipAddress[IP_ADDRESS_BUFFER_SIZE - 1] = '\0';
    inputLen = strnlen(ipAddress, IP_ADDRESS_BUFFER_SIZE);

    if (inputLen > 0 && ipAddress[inputLen - 1] == '\n') {
        ipAddress[inputLen - 1] = '\0';
    } else if (inputLen == IP_ADDRESS_BUFFER_SIZE - 1) {
        int ch;
        do {
            ch = getchar();
        } while (ch != '\n' && ch != EOF);
    }

    removeLeadingZeros(ipAddress, IP_ADDRESS_BUFFER_SIZE);

    printf("IP address without leading zeros: %s\n", ipAddress);

    free(ipAddress);
    return 0;
}