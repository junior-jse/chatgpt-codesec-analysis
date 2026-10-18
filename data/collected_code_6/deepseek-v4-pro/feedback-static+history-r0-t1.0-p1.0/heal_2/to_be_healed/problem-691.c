/* Repair history (previous round):
 * - Flawfinder strncpy was removed from group_tuples() and now appears in safe_strncpy().
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GROUPS 256
#define MAX_KEY_LEN 64
#define MAX_VAL_LEN 64

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char key[MAX_KEY_LEN];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char values[MAX_GROUPS][MAX_VAL_LEN];
    int count;
} Group;

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char first[MAX_VAL_LEN];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char second[MAX_KEY_LEN];
} Tuple;

static int find_group(Group *groups, int group_count, const char *key) {
    int i;
    for (i = 0; i < group_count; i++) {
        if (strcmp(groups[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

static void safe_strncpy(char *dest, const char *src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return;
    }
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0';
}

int group_tuples(Tuple *tuples, int tuple_count, Group *groups, int max_groups) {
    int group_count = 0;
    int i;
    
    if (tuples == NULL || groups == NULL || tuple_count <= 0 || max_groups <= 0 || max_groups > MAX_GROUPS) {
        return -1;
    }
    
    for (i = 0; i < tuple_count; i++) {
        int idx = find_group(groups, group_count, tuples[i].second);
        
        if (idx >= 0) {
            if (groups[idx].count < MAX_GROUPS) {
                safe_strncpy(groups[idx].values[groups[idx].count], tuples[i].first, MAX_VAL_LEN);
                groups[idx].count++;
            }
        } else {
            if (group_count >= max_groups) {
                return -2;
            }
            safe_strncpy(groups[group_count].key, tuples[i].second, MAX_KEY_LEN);
            safe_strncpy(groups[group_count].values[0], tuples[i].first, MAX_VAL_LEN);
            groups[group_count].count = 1;
            group_count++;
        }
    }
    
    return group_count;
}

int main(void) {
    Tuple tuples[] = {
        {"apple", "fruit"},
        {"banana", "fruit"},
        {"carrot", "vegetable"},
        {"dog", "animal"},
        {"elephant", "animal"},
        {"fig", "fruit"},
        {"grape", "fruit"}
    };
    int tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    Group groups[MAX_GROUPS];
    int result;
    int i;
    int j;
    
    result = group_tuples(tuples, tuple_count, groups, MAX_GROUPS);
    
    if (result < 0) {
        printf("Error during grouping\n");
        return 1;
    }
    
    for (i = 0; i < result; i++) {
        printf("%s: ", groups[i].key);
        for (j = 0; j < groups[i].count; j++) {
            if (j > 0) {
                printf(", ");
            }
            printf("%s", groups[i].values[j]);
        }
        printf("\n");
    }
    
    return 0;
}