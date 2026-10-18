#include <stdio.h>

void remove_leading_zeroes(char *ip) {
    if (ip == NULL || *ip == '\0') {
        return;
    }
    char *src = ip;
    char *dest = ip;
    while (*src) {
        char *seg_start = src;
        while (*src && *src != '.') {
            src++;
        }
        char *p = seg_start;
        while (p < src - 1 && *p == '0') {
            p++;
        }
        while (p < src) {
            *dest++ = *p++;
        }
        if (*src == '.') {
            *dest++ = '.';
            src++;
        }
    }
    *dest = '\0';
}

int main(void) {
    char ip1[] = "192.168.001.001";
    char ip2[] = "010.000.000.001";
    char ip3[] = "000.000.000.000";
    char ip4[] = "255.255.255.255";

    remove_leading_zeroes(ip1);
    remove_leading_zeroes(ip2);
    remove_leading_zeroes(ip3);
    remove_leading_zeroes(ip4);

    printf("%s\n", ip1);
    printf("%s\n", ip2);
    printf("%s\n", ip3);
    printf("%s\n", ip4);

    return 0;
}