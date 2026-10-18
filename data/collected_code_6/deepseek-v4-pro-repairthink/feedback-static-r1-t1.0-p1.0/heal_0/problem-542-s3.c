#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

void replace_chars(char *str) {
    regex_t regex;
    regmatch_t match;
    char *result;
    size_t len;
    int reti;
    const char *pattern = "[ ,.]";
    
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
    
    len = strlen(str);
    result = (char *)malloc(len + 1);
    if (result == NULL) {
        regfree(&regex);
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    size_t src_pos = 0;
    size_t dst_pos = 0;
    
    while (src_pos < len) {
        reti = regexec(&regex, str + src_pos, 1, &match, 0);
        if (reti == 0 && match.rm_so == 0) {
            result[dst_pos++] = ':';
            src_pos += match.rm_eo;
        } else {
            result[dst_pos++] = str[src_pos++];
        }
    }
    
    result[dst_pos] = '\0';
    strcpy(str, result);
    
    free(result);
    regfree(&regex);
}

int main() {
    char str1[] = "Hello, world. This is a test";
    char str2[] = "No,special.chars here";
    char str3[] = "   spaces   ";
    
    printf("Original: %s\n", str1);
    replace_chars(str1);
    printf("Modified: %s\n\n", str1);
    
    printf("Original: %s\n", str2);
    replace_chars(str2);
    printf("Modified: %s\n\n", str2);
    
    printf("Original: %s\n", str3);
    replace_chars(str3);
    printf("Modified: %s\n", str3);
    
    return 0;
}