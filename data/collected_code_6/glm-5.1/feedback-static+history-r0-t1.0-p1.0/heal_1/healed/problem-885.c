#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

bool isIsomorphic(const char *s, const char *t) {
    if (s == NULL || t == NULL) {
        return false;
    }

    size_t max_len = 256;
    const char *p1 = s;
    const char *p2 = t;

    while (max_len-- > 0) {
        if (*p1 == '\0' && *p2 == '\0') {
            break;
        }
        if (*p1 == '\0' || *p2 == '\0') {
            return false;
        }
        p1++;
        p2++;
    }

    if (max_len == (size_t)-1) {
        bool s_term = (strnlen(s, max_len + 1) <= max_len);
        bool t_term = (strnlen(t, max_len + 1) <= max_len);
        if (s_term != t_term) {
            return false;
        }
        if (!s_term) {
            return false;
        }
    }

    int map_s_to_t[UCHAR_MAX + 1];
    int map_t_to_s[UCHAR_MAX + 1];

    for (int i = 0; i <= UCHAR_MAX; i++) {
        map_s_to_t[i] = -1;
        map_t_to_s[i] = -1;
    }

    size_t len = strnlen(s, 256);
    for (size_t i = 0; i < len; i++) {
        unsigned char char_s = (unsigned char)s[i];
        unsigned char char_t = (unsigned char)t[i];

        if (map_s_to_t[char_s] == -1) {
            if (map_t_to_s[char_t] != -1) {
                return false;
            }
            map_s_to_t[char_s] = char_t;
            map_t_to_s[char_t] = char_s;
        } else {
            if (map_s_to_t[char_s] != char_t) {
                return false;
            }
        }
    }

    return true;
}

int main(int argc, char *argv[]) {
    const char *s1 = "egg";
    const char *t1 = "add";
    bool res1 = isIsomorphic(s1, t1);
    printf("%d\n", res1);

    const char *s2 = "foo";
    const char *t2 = "bar";
    bool res2 = isIsomorphic(s2, t2);
    printf("%d\n", res2);

    const char *s3 = "paper";
    const char *t3 = "title";
    bool res3 = isIsomorphic(s3, t3);
    printf("%d\n", res3);

    bool res4 = isIsomorphic(NULL, "test");
    printf("%d\n", res4);

    bool res5 = isIsomorphic("a", "ab");
    printf("%d\n", res5);

    bool res6 = isIsomorphic("ab", "a");
    printf("%d\n", res6);

    return 0;
}