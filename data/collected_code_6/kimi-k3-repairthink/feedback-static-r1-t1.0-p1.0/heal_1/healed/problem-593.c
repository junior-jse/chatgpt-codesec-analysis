#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void removeLeadingZeros(char *ipAddress, size_t ipAddressSize) {
    char *token;
    char *saveptr;
    char *result;
    size_t resultSize;
    size_t resultLen = 0;

    if (ipAddress == NULL || ipAddressSize == 0) {
        return;
    }

    ipAddress[ipAddressSize - 1] = '\0';

    resultSize = strlen(ipAddress) + 1;
    result = (char *)malloc(resultSize);
    if (result == NULL) {
        return;
    }
    result[0] = '\0';

    token = strtok_r(ipAddress, ".", &saveptr);
    while (token != NULL) {
        char *temp = token;
        size_t tokenLen;

        while (*temp == '0') {
            temp++;
        }
        if (*temp == '\0') {
            temp--;
        }

        tokenLen = strlen(temp);
        if (resultLen + tokenLen + 2 > resultSize) {
            break;
        }

        memcpy(result + resultLen, temp, tokenLen);
        resultLen += tokenLen;
        result[resultLen] = '.';
        resultLen++;
        result[resultLen] = '\0';

        token = strtok_r(NULL, ".", &saveptr);
    }

    if (resultLen > 0) {
        result[resultLen - 1] = '\0';
    }

    snprintf(ipAddress, ipAddressSize, "%s", result);
    free(result);
}

int main(void) {
    char ipAddress[100];

    printf("Enter an IP address: ");
    if (fgets(ipAddress, sizeof(ipAddress), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    ipAddress[strcspn(ipAddress, "\n")] = '\0';

    removeLeadingZeros(ipAddress, sizeof(ipAddress));

    printf("IP address without leading zeros: %s\n", ipAddress);

    return 0;
}