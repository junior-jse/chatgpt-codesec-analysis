#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>
#include <limits.h>

typedef struct {
    int year;
    int month;
    int day;
} DateComponents;

static int parse_int_safe(const char *str, int *out) {
    char *endptr;
    long value;

    if (str == NULL || *str == '\0') {
        return -1;
    }

    errno = 0;
    value = strtol(str, &endptr, 10);
    if (errno != 0 || endptr == str || *endptr != '\0') {
        return -1;
    }
    if (value < INT_MIN || value > INT_MAX) {
        return -1;
    }

    *out = (int)value;
    return 0;
}

int extract_date_from_url(const char *url, DateComponents *date) {
    regex_t regex;
    regmatch_t matches[4];
    const char *pattern = "/([0-9]{4})/([0-9]{2})/([0-9]{2})/";
    int ret;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char year_str[5], month_str[3], day_str[3];

    if (url == NULL || date == NULL) {
        return -1;
    }

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return -1;
    }

    ret = regexec(&regex, url, 4, matches, 0);
    if (ret != 0) {
        regfree(&regex);
        return -1;
    }

    if (matches[1].rm_so == -1 || matches[2].rm_so == -1 || matches[3].rm_so == -1) {
        regfree(&regex);
        return -1;
    }

    int year_len = (int)(matches[1].rm_eo - matches[1].rm_so);
    int month_len = (int)(matches[2].rm_eo - matches[2].rm_so);
    int day_len = (int)(matches[3].rm_eo - matches[3].rm_so);

    if (year_len != 4 || month_len != 2 || day_len != 2) {
        regfree(&regex);
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(year_str, url + matches[1].rm_so, 4);
    year_str[4] = '\0';
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(month_str, url + matches[2].rm_so, 2);
    month_str[2] = '\0';
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(day_str, url + matches[3].rm_so, 2);
    day_str[2] = '\0';

    if (parse_int_safe(year_str, &date->year) != 0 ||
        parse_int_safe(month_str, &date->month) != 0 ||
        parse_int_safe(day_str, &date->day) != 0) {
        regfree(&regex);
        return -1;
    }

    if (date->year < 1900 || date->year > 2100 ||
        date->month < 1 || date->month > 12 ||
        date->day < 1 || date->day > 31) {
        regfree(&regex);
        return -1;
    }

    regfree(&regex);
    return 0;
}

int main(void) {
    const char *test_urls[] = {
        "https://example.com/2024/01/15/article",
        "https://example.com/2023/12/25/news",
        "https://example.com/invalid/path",
        NULL
    };

    for (int i = 0; test_urls[i] != NULL; i++) {
        DateComponents date;
        int result = extract_date_from_url(test_urls[i], &date);

        if (result == 0) {
            printf("URL: %s\n", test_urls[i]);
            printf("Year: %d, Month: %d, Day: %d\n\n", date.year, date.month, date.day);
        } else {
            printf("URL: %s\n", test_urls[i]);
            printf("Failed to extract date\n\n");
        }
    }

    return 0;
}