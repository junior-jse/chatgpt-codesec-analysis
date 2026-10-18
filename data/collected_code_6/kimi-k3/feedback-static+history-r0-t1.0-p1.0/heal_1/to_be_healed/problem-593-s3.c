#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void removeLeadingZeros(char *ip) {
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    if (ip == NULL || strlen(ip) == 0) {
        return;
    }

    char *token;
    char *rest = ip;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char result[16] = {0};
    int first = 1;

    while ((token = strtok_r(rest, ".", &rest)) != NULL) {
        while (*token == '0' && *(token + 1) != '\0') {
            token++;
        }

        if (!first) {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             * Flawfinder strncat: Easily used incorrectly (e.g., incorrectly computing the correct maximum size to add) [MS-banned] (CWE-120). Consider strcat_s, strlcat, snprintf, or automatically resizing strings. Risk is low because the source is a constant character. (risk 1, buffer)
             */
            strncat(result, ".", sizeof(result) - strlen(result) - 1);
        }
        /* Possible weaknesses found:
         * Flawfinder strncat: Easily used incorrectly (e.g., incorrectly computing the correct maximum size to add) [MS-banned] (CWE-120). Consider strcat_s, strlcat, snprintf, or automatically resizing strings. (risk 1, buffer)
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        strncat(result, token, sizeof(result) - strlen(result) - 1);
        first = 0;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    if (strlen(result) > 0) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
         */
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