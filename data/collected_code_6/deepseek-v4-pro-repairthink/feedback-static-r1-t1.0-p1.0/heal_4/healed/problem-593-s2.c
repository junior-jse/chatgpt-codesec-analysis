#include <stdio.h>
#include <string.h>

void remove_leading_zeros(char *ip) {
    if (ip == NULL) return;

    char *read = ip;
    char *write = ip;

    while (*read) {
        char *dot = strchr(read, '.');
        if (dot == NULL) {
            dot = read + strlen(read);
        }

        char *start = read;
        while (start < dot - 1 && *start == '0') {
            start++;
        }

        size_t len = dot - start;
        memmove(write, start, len);
        write += len;

        if (*dot == '.') {
            *write++ = '.';
            read = dot + 1;
        } else {
            read = dot;
        }
    }
    *write = '\0';
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