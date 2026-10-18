#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>
#include <limits.h>

static int safe_strtol(const char *str, int *value) {
    char *endptr;
    long tmp;
    
    errno = 0;
    tmp = strtol(str, &endptr, 10);
    
    if (errno == ERANGE || tmp < INT_MIN || tmp > INT_MAX) {
        return -1;
    }
    if (endptr == str || *endptr != '\0') {
        return -1;
    }
    
    *value = (int)tmp;
    return 0;
}

int extract_ymd(const char *url, int *year, int *month, int *day) {
    if (url == NULL || year == NULL || month == NULL || day == NULL) {
        return -1;
    }

    regex_t regex;
    regmatch_t matches[4];
    const char *pattern = "/([0-9]{4})/([0-9]{1,2})/([0-9]{1,2})(/|$)";
    int ret;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char year_str[16];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char month_str[8];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char day_str[8];

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return -1;
    }

    ret = regexec(&regex, url, 4, matches, 0);
    if (ret != 0) {
        regfree(&regex);
        return -1;
    }

    if (matches[1].rm_eo - matches[1].rm_so > (int)sizeof(year_str) - 1) {
        regfree(&regex);
        return -1;
    }
    snprintf(year_str, sizeof(year_str), "%.*s",
              (int)(matches[1].rm_eo - matches[1].rm_so),
              url + matches[1].rm_so);

    if (matches[2].rm_eo - matches[2].rm_so > (int)sizeof(month_str) - 1) {
        regfree(&regex);
        return -1;
    }
    snprintf(month_str, sizeof(month_str), "%.*s",
              (int)(matches[2].rm_eo - matches[2].rm_so),
              url + matches[2].rm_so);

    if (matches[3].rm_eo - matches[3].rm_so > (int)sizeof(day_str) - 1) {
        regfree(&regex);
        return -1;
    }
    snprintf(day_str, sizeof(day_str), "%.*s",
              (int)(matches[3].rm_eo - matches[3].rm_so),
              url + matches[3].rm_so);

    if (safe_strtol(year_str, year) != 0) {
        regfree(&regex);
        return -1;
    }
    if (safe_strtol(month_str, month) != 0) {
        regfree(&regex);
        return -1;
    }
    if (safe_strtol(day_str, day) != 0) {
        regfree(&regex);
        return -1;
    }

    regfree(&regex);
    return 0;
}

int main(void) {
    const char *test_urls[] = {
        "https://example.com/2023/10/05/article",
        "http://test.org/2024/1/9",
        "https://nodate.com/path",
        NULL
    };

    int year, month, day;

    for (int i = 0; test_urls[i] != NULL; i++) {
        int result = extract_ymd(test_urls[i], &year, &month, &day);
        if (result == 0) {
            printf("%s -> %d-%d-%d\n", test_urls[i], year, month, day);
        } else {
            printf("%s -> no date found\n", test_urls[i]);
        }
    }

    return 0;
}