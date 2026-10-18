#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_reverse(const char *a, const char *b) {
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    
    if (len_a != len_b) {
        return 0;
    }
    
    for (size_t i = 0; i < len_a; i++) {
        if (a[i] != b[len_b - 1 - i]) {
            return 0;
        }
    }
    
    return 1;
}

int count_reverse_pairs(char **strings, int count) {
    if (strings == NULL || count <= 0) {
        return 0;
    }
    
    int pair_count = 0;
    int *used = (int *)calloc(count, sizeof(int));
    
    if (used == NULL) {
        return 0;
    }
    
    for (int i = 0; i < count; i++) {
        if (strings[i] == NULL) {
            continue;
        }
        
        for (int j = i + 1; j < count; j++) {
            if (strings[j] == NULL || used[i] || used[j]) {
                continue;
            }
            
            if (is_reverse(strings[i], strings[j])) {
                used[i] = 1;
                used[j] = 1;
                pair_count++;
                break;
            }
        }
    }
    
    free(used);
    return pair_count;
}

int main(void) {
    char *strings1[] = {"abc", "cba", "def", "fed", "xyz"};
    int count1 = sizeof(strings1) / sizeof(strings1[0]);
    printf("Test 1: %d\n", count_reverse_pairs(strings1, count1));
    
    char *strings2[] = {"hello", "world", "dlrow", "olleh", "test"};
    int count2 = sizeof(strings2) / sizeof(strings2[0]);
    printf("Test 2: %d\n", count_reverse_pairs(strings2, count2));
    
    char *strings3[] = {"a", "b", "c"};
    int count3 = sizeof(strings3) / sizeof(strings3[0]);
    printf("Test 3: %d\n", count_reverse_pairs(strings3, count3));
    
    char *strings4[] = {"", "aa", "aa", ""};
    int count4 = sizeof(strings4) / sizeof(strings4[0]);
    printf("Test 4: %d\n", count_reverse_pairs(strings4, count4));
    
    char *strings5[] = {"race", "ecar", "race", "ecar"};
    int count5 = sizeof(strings5) / sizeof(strings5[0]);
    printf("Test 5: %d\n", count_reverse_pairs(strings5, count5));
    
    printf("Test 6: %d\n", count_reverse_pairs(NULL, 5));
    printf("Test 7: %d\n", count_reverse_pairs(strings5, 0));
    
    return 0;
}