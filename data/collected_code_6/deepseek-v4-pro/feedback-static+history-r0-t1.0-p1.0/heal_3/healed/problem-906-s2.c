#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>
#include <limits.h>

static int safe_strtol(const char *str, int *value) {
    char *endptr;
    long tmp;
    
    if (str == NULL || *str == '\0') {
        return -1;
    }
    
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

    size_t url_len = strnlen(url, 2049);
    if (url_len == 0 || url_len > 2048) {
        return -1;
    }

    regex_t regex;
    regmatch_t matches[4];
    const char *pattern = "/([0-9]{4})/([0-9]{1,2})/([0-9]{1,2})(/|$)";
    int ret;

    char year_str[5] = {0};
    char month_str[3] = {0};
    char day_str[3] = {0};

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return -1;
    }

    ret = regexec(&regex, url, 4, matches, 0);
    if (ret != 0) {
        regfree(&regex);
        return -1;
    }

    if (matches[1].rm_so < 0 || matches[2].rm_so < 0 || matches[3].rm_so < 0) {
        regfree(&regex);
        return -1;
    }

    size_t year_len = (size_t)(matches[1].rm_eo - matches[1].rm_so);
    if (year_len >= sizeof(year_str) || year_len < 1) {
        regfree(&regex);
        return -1;
    }
    if ((size_t)(matches[1].rm_so + year_len) > url_len) {
        regfree(&regex);
        return -1;
    }
    memcpy(year_str, url + matches[1].rm_so, year_len);
    year_str[year_len] = '\0';

    size_t month_len = (size_t)(matches[2].rm_eo - matches[2].rm_so);
    if (month_len >= sizeof(month_str) || month_len < 1) {
        regfree(&regex);
        return -1;
    }
    if ((size_t)(matches[2].rm_so + month_len) > url_len) {
        regfree(&regex);
        return -1;
    }
    memcpy(month_str, url + matches[2].rm_so, month_len);
    month_str[month_len] = '\0';

    size_t day_len = (size_t)(matches[3].rm_eo - matches[3].rm_so);
    if (day_len >= sizeof(day_str) || day_len < 1) {
        regfree(&regex);
        return -1;
    }
    if ((size_t)(matches[3].rm_so + day_len) > url_len) {
        regfree(&regex);
        return -1;
    }
    memcpy(day_str, url + matches[3].rm_so, day_len);
    day_str[day_len] = '\0';

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

    if (*month < 1 || *month > 12 || *day < 1 || *day > 31) {
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