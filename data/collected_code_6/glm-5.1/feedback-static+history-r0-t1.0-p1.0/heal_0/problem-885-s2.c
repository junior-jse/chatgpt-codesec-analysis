#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHARS 256

bool areIsomorphic(const char *s, const char *t) {
    if (s == NULL || t == NULL) {
        return false;
    }

    size_t len1 = strlen(s);
    size_t len2 = strlen(t);

    if (len1 != len2) {
        return false;
    }

    int map_st[MAX_CHARS];
    int map_ts[MAX_CHARS];

    for (int i = 0; i < MAX_CHARS; i++) {
        map_st[i] = -1;
        map_ts[i] = -1;
    }

    for (size_t i = 0; i < len1; i++) {
        unsigned char c1 = (unsigned char)s[i];
        unsigned char c2 = (unsigned char)t[i];

        if (map_st[c1] == -1 && map_ts[c2] == -1) {
            map_st[c1] = c2;
            map_ts[c2] = c1;
        } else {
            if (map_st[c1] != c2 || map_ts[c2] != c1) {
                return false;
            }
        }
    }

    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <string1> <string2>\n", argv[0]);
        return 1;
    }

    bool result = areIsomorphic(argv[1], argv[2]);
    
    printf("%s\n", result ? "Isomorphic" : "Not Isomorphic");

    return 0;
}