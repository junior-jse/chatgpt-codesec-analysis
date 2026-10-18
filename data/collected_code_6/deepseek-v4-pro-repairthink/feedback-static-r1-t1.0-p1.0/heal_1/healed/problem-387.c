#include <stdio.h>
#include <ctype.h>

int isEvenHex(const char *hex) {
    if (hex == NULL || *hex == '\0') {
        return -1;
    }

    const char *p = hex;
    while (*p) {
        if (!isxdigit((unsigned char)*p)) {
            return -1;
        }
        p++;
    }

    size_t len = p - hex;
    char last = hex[len - 1];

    int value;
    if (last >= '0' && last <= '9') {
        value = last - '0';
    } else {
        value = 10 + (tolower((unsigned char)last) - 'a');
    }

    return (value % 2 == 0) ? 1 : 0;
}

int main(void) {
    const char *test1 = "1A";
    const char *test2 = "2F";
    const char *test3 = "123";
    const char *test4 = "XYZ";
    const char *test5 = "";

    printf("%s: %d\n", test1, isEvenHex(test1));
    printf("%s: %d\n", test2, isEvenHex(test2));
    printf("%s: %d\n", test3, isEvenHex(test3));
    printf("%s: %d\n", test4, isEvenHex(test4));
    printf("\"%s\": %d\n", test5, isEvenHex(test5));

    return 0;
}