#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void removeLeadingZeros(char *ipAddress) {
    char *token;
    char *saveptr;
    char *temp;
    char *result = (char *)malloc(strlen(ipAddress) + 1);
    if (result == NULL) {
        return;
    }
    result[0] = '\0';

    token = strtok_r(ipAddress, ".", &saveptr);
    while (token != NULL) {
        temp = token;
        while (*temp == '0') {
            temp++;
        }
        if (*temp == '\0') {
            temp--;
        }
        strcat(result, temp);
        strcat(result, ".");
        token = strtok_r(NULL, ".", &saveptr);
    }

    result[strlen(result) - 1] = '\0';
    strcpy(ipAddress, result);
    free(result);
}

int main() {
    char ipAddress[100];
    printf("Enter an IP address: ");
    fgets(ipAddress, sizeof(ipAddress), stdin);
    ipAddress[strcspn(ipAddress, "\n")] = '\0';

    removeLeadingZeros(ipAddress);

    printf("IP address without leading zeros: %s\n", ipAddress);

    return 0;
}