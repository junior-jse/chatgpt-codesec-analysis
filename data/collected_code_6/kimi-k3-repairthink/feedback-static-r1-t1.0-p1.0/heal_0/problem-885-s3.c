#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool isIsomorphic(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) {
        return false;
    }
    
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    
    if (len1 != len2) {
        return false;
    }
    
    int map1[256] = {0};
    int map2[256] = {0};
    
    for (size_t i = 0; i < len1; i++) {
        unsigned char c1 = (unsigned char)s1[i];
        unsigned char c2 = (unsigned char)s2[i];
        
        if (map1[c1] == 0 && map2[c2] == 0) {
            map1[c1] = c2 + 1;
            map2[c2] = c1 + 1;
        } else if (map1[c1] != c2 + 1 || map2[c2] != c1 + 1) {
            return false;
        }
    }
    
    return true;
}

int main(void) {
    const char *str1 = "egg";
    const char *str2 = "add";
    
    if (isIsomorphic(str1, str2)) {
        printf("true\n");
    } else {
        printf("false\n");
    }
    
    return 0;
}