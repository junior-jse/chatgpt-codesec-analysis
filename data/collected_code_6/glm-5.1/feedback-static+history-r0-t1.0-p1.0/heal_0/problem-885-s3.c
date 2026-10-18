#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isIsomorphic(const char *s, const char *t) {
    int map_s[256];
    int map_t[256];
    for (int i = 0; i < 256; i++) {
        map_s[i] = -1;
        map_t[i] = -1;
    }
    int len = strlen(s);
    if (len != strlen(t)) {
        return 0;
    }
    for (int i = 0; i < len; i++) {
        unsigned char char_s = s[i];
        unsigned char char_t = t[i];
        if (map_s[char_s] == -1 && map_t[char_t] == -1) {
            map_s[char_s] = char_t;
            map_t[char_t] = char_s;
        } else {
            if (map_s[char_s] != char_t || map_t[char_t] != char_s) {
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    const char *s1 = "egg";
    const char *t1 = "add";
    printf("%d\n", isIsomorphic(s1, t1));

    const char *s2 = "foo";
    const char *t2 = "bar";
    printf("%d\n", isIsomorphic(s2, t2));

    const char *s3 = "paper";
    const char *t3 = "title";
    printf("%d\n", isIsomorphic(s3, t3));

    return 0;
}