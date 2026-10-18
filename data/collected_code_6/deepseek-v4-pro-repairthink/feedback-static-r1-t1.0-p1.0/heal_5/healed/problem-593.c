#include <stdio.h>

void remove_leading_zeroes(char *ip) {
    if (ip == NULL) return;
    char *read = ip;
    char *write = ip;
    while (*read) {
        if (*read == '.') {
            *write++ = '.';
            read++;
            continue;
        }
        char *octet_start = read;
        while (*read >= '0' && *read <= '9') {
            read++;
        }
        char *p = octet_start;
        while (p < read && *p == '0') {
            p++;
        }
        if (p == read) {
            p--;
        }
        while (p < read) {
            *write++ = *p++;
        }
    }
    *write = '\0';
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