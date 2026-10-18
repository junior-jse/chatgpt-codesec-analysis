#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

bool all_unique(const char *str) {
    if (str == NULL) {
        return false;
    }
    
    bool seen[UCHAR_MAX + 1] = {false};
    size_t len = strlen(str);
    
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (seen[c]) {
            return false;
        }
        seen[c] = true;
    }
    
    return true;
}

int main(void) {
    char buffer[1024];
    
    printf("Enter a string: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (all_unique(buffer)) {
        printf("All characters are unique.\n");
    } else {
        printf("Duplicate characters found.\n");
    }
    
    return 0;
}