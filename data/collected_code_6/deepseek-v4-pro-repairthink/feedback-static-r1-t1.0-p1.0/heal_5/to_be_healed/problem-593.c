#include <stdio.h>

void remove_leading_zeroes(char *ip) {
    if (ip == NULL) return;
    int write = 0;
    int octet_start = 1;
    /* Possible weaknesses found:
     * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    for (int read = 0; ip[read] != '\0'; read++) {
        /* Possible weaknesses found:
         * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        char c = ip[read];
        if (c == '.') {
            ip[write++] = '.';
            octet_start = 1;
        } else if (c >= '0' && c <= '9') {
            if (octet_start) {
                if (c == '0' && ip[read+1] >= '0' && ip[read+1] <= '9') {
                    continue;
                }
                octet_start = 0;
            }
            ip[write++] = c;
        }
    }
    ip[write] = '\0';
}

int main(void) {
    char ip1[] = "192.168.001.001";
    char ip2[] = "010.000.000.001";
    char ip3[] = "000.000.000.000";
    char ip4[] = "255.255.255.255";
    char ip5[] = "001.002.003.004";

    remove_leading_zeroes(ip1);
    remove_leading_zeroes(ip2);
    remove_leading_zeroes(ip3);
    remove_leading_zeroes(ip4);
    remove_leading_zeroes(ip5);

    printf("%s\n", ip1);
    printf("%s\n", ip2);
    printf("%s\n", ip3);
    printf("%s\n", ip4);
    printf("%s\n", ip5);

    return 0;
}