#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_IP_LENGTH 16
#define MAX_OCTET_LENGTH 4

bool isValidOctet(const char *octet, size_t len) {
    if (len == 0 || len > 3) {
        return false;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (octet[i] < '0' || octet[i] > '9') {
            return false;
        }
    }
    
    if (len == 3) {
        int value = (octet[0] - '0') * 100 + (octet[1] - '0') * 10 + (octet[2] - '0');
        if (value > 255) {
            return false;
        }
    }
    
    return true;
}

int removeLeadingZeros(const char *input, char *output, size_t outputSize) {
    if (input == NULL || output == NULL || outputSize < MAX_IP_LENGTH) {
        return -1;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t inputLen = strlen(input);
    if (inputLen == 0 || inputLen >= MAX_IP_LENGTH) {
        return -1;
    }
    
    size_t outIndex = 0;
    size_t i = 0;
    int octetCount = 0;
    
    while (i < inputLen && octetCount < 4) {
        size_t octetStart = i;
        
        while (i < inputLen && input[i] != '.') {
            i++;
        }
        
        size_t octetLen = i - octetStart;
        
        if (!isValidOctet(input + octetStart, octetLen)) {
            return -1;
        }
        
        size_t firstNonZero = octetStart;
        while (firstNonZero < i - 1 && input[firstNonZero] == '0') {
            firstNonZero++;
        }
        
        size_t digitsToCopy = i - firstNonZero;
        
        if (outIndex + digitsToCopy + 1 >= outputSize) {
            return -1;
        }
        
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(output + outIndex, input + firstNonZero, digitsToCopy);
        outIndex += digitsToCopy;
        
        if (i < inputLen && input[i] == '.') {
            if (octetCount < 3) {
                output[outIndex++] = '.';
            }
            i++;
        }
        
        octetCount++;
    }
    
    if (octetCount != 4 || i != inputLen) {
        return -1;
    }
    
    output[outIndex] = '\0';
    return 0;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char output[MAX_IP_LENGTH];
    const char *testCases[] = {
        "192.168.001.001",
        "010.000.000.001",
        "255.255.255.255",
        "000.000.000.000",
        "001.002.003.004"
    };
    
    size_t numTests = sizeof(testCases) / sizeof(testCases[0]);
    
    for (size_t i = 0; i < numTests; i++) {
        memset(output, 0, sizeof(output));
        
        if (removeLeadingZeros(testCases[i], output, sizeof(output)) == 0) {
            printf("Input:  %s\n", testCases[i]);
            printf("Output: %s\n\n", output);
        } else {
            printf("Input:  %s\n", testCases[i]);
            printf("Output: Invalid IP address\n\n");
        }
    }
    
    return 0;
}