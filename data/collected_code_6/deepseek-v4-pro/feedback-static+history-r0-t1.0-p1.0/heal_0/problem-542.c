#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

void replace_chars(char *str) {
    regex_t regex;
    regmatch_t match;
    char *src = str;
    char *dst = str;
    int ret;
    
    ret = regcomp(&regex, "[ ,.]", REG_EXTENDED);
    if (ret) {
        return;
    }
    
    while (*src) {
        if (regexec(&regex, src, 1, &match, 0) == 0 && match.rm_so == 0) {
            *dst++ = ':';
            src += match.rm_eo;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
    
    regfree(&regex);
}

int main(void) {
    char str[] = "Hello, world. This is a test";
    
    replace_chars(str);
    printf("%s\n", str);
    
    return 0;
}