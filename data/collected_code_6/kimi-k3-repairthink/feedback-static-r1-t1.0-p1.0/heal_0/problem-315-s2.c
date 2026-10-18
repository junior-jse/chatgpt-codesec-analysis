#include <stdio.h>
#include <string.h>
#include <ctype.h>

int first_max_even_word_length(const char *str)
{
    if (str == NULL) {
        return -1;
    }
    
    int max_len = 0;
    int current_len = 0;
    int in_word = 0;
    
    for (size_t i = 0; ; i++) {
        if (str[i] != '\0' && !isspace((unsigned char)str[i])) {
            if (!in_word) {
                in_word = 1;
                current_len = 1;
            } else {
                current_len++;
            }
        } else {
            if (in_word && current_len % 2 == 0) {
                if (current_len > max_len) {
                    max_len = current_len;
                }
            }
            in_word = 0;
            current_len = 0;
            
            if (str[i] == '\0') {
                break;
            }
        }
    }
    
    return max_len;
}

int main(void)
{
    const char *test1 = "Hello world this is a test string";
    const char *test2 = "ab cd ef gh ij";
    const char *test3 = "odd words only here";
    const char *test4 = "";
    const char *test5 = NULL;
    
    printf("Test 1: %d\n", first_max_even_word_length(test1));
    printf("Test 2: %d\n", first_max_even_word_length(test2));
    printf("Test 3: %d\n", first_max_even_word_length(test3));
    printf("Test 4: %d\n", first_max_even_word_length(test4));
    printf("Test 5: %d\n", first_max_even_word_length(test5));
    
    return 0;
}