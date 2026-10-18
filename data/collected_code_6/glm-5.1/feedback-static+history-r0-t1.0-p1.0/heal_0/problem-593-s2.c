#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *remove_leading_zeroes_ip(const char *ip) {
    if (ip == NULL) {
        return NULL;
    }

    size_t len = strlen(ip);
    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t res_idx = 0;
    size_t i = 0;

    while (ip[i] != '\0') {
        if (ip[i] == '0' && (i == 0 || ip[i - 1] == '.')) {
            if (ip[i + 1] != '\0' && ip[i + 1] != '.') {
                i++;
            } else {
                result[res_idx++] = ip[i++];
            }
        } else {
            result[res_idx++] = ip[i++];
        }
    }

    result[res_idx] = '\0';
    return result;
}

int main(void) {
    char ip1[] = "010.000.001.000";
    char *res1 = remove_leading_zeroes_ip(ip1);
    if (res1 != NULL) {
        printf("%s\n", res1);
        free(res1);
    }

    char ip2[] = "192.168.1.1";
    char *res2 = remove_leading_zeroes_ip(ip2);
    if (res2 != NULL) {
        printf("%s\n", res2);
        free(res2);
    }

    char ip3[] = "000.000.000.000";
    char *res3 = remove_leading_zeroes_ip(ip3);
    if (res3 != NULL) {
        printf("%s\n", res3);
        free(res3);
    }

    return 0;
}