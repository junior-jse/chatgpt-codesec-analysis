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

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
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

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        tokenLen = strlen(temp);
        if (resultLen + tokenLen + 2 > resultSize) {
            break;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
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
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
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