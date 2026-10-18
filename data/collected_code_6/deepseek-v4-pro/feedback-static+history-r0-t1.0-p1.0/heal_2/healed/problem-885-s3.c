#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHARS 256

bool is_isomorphic(const char *s, const char *t) {
    if (s == NULL || t == NULL) {
        return false;
    }

    int map_s_to_t[MAX_CHARS] = {0};
    int map_t_to_s[MAX_CHARS] = {0};

    size_t len_s = 0;
    size_t len_t = 0;

    while (s[len_s] != '\0' && t[len_t] != '\0') {
        unsigned char c1 = (unsigned char)s[len_s];
        unsigned char c2 = (unsigned char)t[len_t];

        if (map_s_to_t[c1] == 0 && map_t_to_s[c2] == 0) {
            map_s_to_t[c1] = c2;
            map_t_to_s[c2] = c1;
        } else if (map_s_to_t[c1] != c2 || map_t_to_s[c2] != c1) {
            return false;
        }

        len_s++;
        len_t++;
    }

    if (s[len_s] != '\0' || t[len_t] != '\0') {
        return false;
    }

    return true;
}

int main(void) {
    const char *str1 = "egg";
    const char *str2 = "add";
    const char *str3 = "foo";
    const char *str4 = "bar";
    const char *str5 = "paper";
    const char *str6 = "title";

    printf("%s and %s: %s\n", str1, str2, is_isomorphic(str1, str2) ? "true" : "false");
    printf("%s and %s: %s\n", str3, str4, is_isomorphic(str3, str4) ? "true" : "false");
    printf("%s and %s: %s\n", str5, str6, is_isomorphic(str5, str6) ? "true" : "false");

    return 0;
}