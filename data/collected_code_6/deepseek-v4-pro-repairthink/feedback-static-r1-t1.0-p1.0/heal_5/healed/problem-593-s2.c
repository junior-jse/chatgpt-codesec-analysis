#include <stdio.h>

void remove_leading_zeros(char *ip) {
    if (ip == NULL) return;
    char *src = ip, *dst = ip;
    int in_octet = 0, seen_non_zero = 0;
    while (*src) {
        if (*src == '.') {
            if (!in_octet) *dst++ = '0';
            *dst++ = '.';
            in_octet = 0;
            seen_non_zero = 0;
            src++;
        } else {
            if (!in_octet) {
                in_octet = 1;
                seen_non_zero = 0;
            }
            if (*src != '0' || seen_non_zero || src[1] == '.' || src[1] == '\0') {
                if (*src != '0') seen_non_zero = 1;
                *dst++ = *src;
            }
            src++;
        }
    }
    *dst = '\0';
}

int main(void) {
    char ip1[] = "192.168.001.001";
    remove_leading_zeros(ip1);
    printf("%s\n", ip1);

    char ip2[] = "010.000.000.010";
    remove_leading_zeros(ip2);
    printf("%s\n", ip2);

    char ip3[] = "0.0.0.0";
    remove_leading_zeros(ip3);
    printf("%s\n", ip3);

    char ip4[] = "255.255.255.255";
    remove_leading_zeros(ip4);
    printf("%s\n", ip4);

    return 0;
}