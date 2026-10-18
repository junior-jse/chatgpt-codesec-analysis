#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void removeLeadingZeros(char *ipAddress) {
    char *token;
    char *saveptr;
    char *result;
    size_t ipLen;
    size_t resultLen = 0;
    size_t maxResultLen;
    
    if (ipAddress == NULL) {
        return;
    }
    
    ipLen = strnlen(ipAddress, 100);
    if (ipLen == 0 || ipLen >= 100) {
        return;
    }
    
    maxResultLen = ipLen;
    
    result = (char *)malloc(maxResultLen + 1);
    if (result == NULL) {
        return;
    }
    result[0] = '\0';

    token = strtok_r(ipAddress, ".", &saveptr);
    while (token != NULL) {
        char *temp = token;
        size_t tempLen;
        
        while (*temp == '0' && *(temp + 1) != '\0') {
            temp++;
        }
        
        tempLen = strnlen(temp, ipLen);
        
        /* Possible weaknesses found:
         *  outer condition: resultLen+tempLen<=maxResultLen
         */
        if (resultLen + tempLen <= maxResultLen) {
            /* Possible weaknesses found:
             *  identical inner condition: resultLen+tempLen<=maxResultLen
             *  Identical inner 'if' condition is always true. [identicalInnerCondition]
             */
            if (resultLen + tempLen <= maxResultLen) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + resultLen, temp, tempLen);
                resultLen += tempLen;
                result[resultLen] = '\0';
                
                if (resultLen < maxResultLen) {
                    result[resultLen] = '.';
                    resultLen++;
                    result[resultLen] = '\0';
                }
            }
        }
        
        token = strtok_r(NULL, ".", &saveptr);
    }

    if (resultLen > 0 && result[resultLen - 1] == '.') {
        result[resultLen - 1] = '\0';
        resultLen--;
    }
    
    if (resultLen < 100 && resultLen <= maxResultLen) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(ipAddress, result, resultLen + 1);
    }
    
    free(result);
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char ipAddress[100];
    
    printf("Enter an IP address: ");
    
    if (fgets(ipAddress, sizeof(ipAddress), stdin) == NULL) {
        return 1;
    }
    
    ipAddress[strcspn(ipAddress, "\n")] = '\0';

    removeLeadingZeros(ipAddress);

    printf("IP address without leading zeros: %s\n", ipAddress);

    return 0;
}