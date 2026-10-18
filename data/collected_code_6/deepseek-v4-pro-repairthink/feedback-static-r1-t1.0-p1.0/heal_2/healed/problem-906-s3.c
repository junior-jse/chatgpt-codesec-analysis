#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

int extract_date_from_url(const char *url, int *year, int *month, int *day) {
    if (url == NULL || year == NULL || month == NULL || day == NULL) {
        return -1;
    }

    regex_t regex;
    regmatch_t matches[4];
    const char *pattern = "([0-9]{4})/([0-9]{1,2})/([0-9]{1,2})";
    int ret;

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return -2;
    }

    ret = regexec(&regex, url, 4, matches, 0);
    if (ret != 0) {
        regfree(&regex);
        return -3;
    }

    int year_len = matches[1].rm_eo - matches[1].rm_so;
    int month_len = matches[2].rm_eo - matches[2].rm_so;
    int day_len = matches[3].rm_eo - matches[3].rm_so;

    if (year_len <= 0 || year_len > 4 || month_len <= 0 || month_len > 2 || day_len <= 0 || day_len > 2) {
        regfree(&regex);
        return -4;
    }

    long y = 0;
    for (int i = 0; i < year_len; i++) {
        y = y * 10 + (url[matches[1].rm_so + i] - '0');
    }
    if (y < 1 || y > 9999) {
        regfree(&regex);
        return -5;
    }

    long m = 0;
    for (int i = 0; i < month_len; i++) {
        m = m * 10 + (url[matches[2].rm_so + i] - '0');
    }
    if (m < 1 || m > 12) {
        regfree(&regex);
        return -5;
    }

    long d = 0;
    for (int i = 0; i < day_len; i++) {
        d = d * 10 + (url[matches[3].rm_so + i] - '0');
    }
    if (d < 1 || d > 31) {
        regfree(&regex);
        return -5;
    }

    *year = (int)y;
    *month = (int)m;
    *day = (int)d;

    regfree(&regex);
    return 0;
}

int main(void) {
    const char *test_urls[] = {
        "https://example.com/2024/03/15/article",
        "http://test.org/2023/12/01/post",
        "https://site.com/2022/7/5/news",
        "https://invalid.com/no/date/here",
        NULL
    };

    int year, month, day;

    for (int i = 0; test_urls[i] != NULL; i++) {
        printf("URL: %s\n", test_urls[i]);
        int result = extract_date_from_url(test_urls[i], &year, &month, &day);
        if (result == 0) {
            printf("Extracted date: %04d-%02d-%02d\n", year, month, day);
        } else {
            printf("Failed to extract date (error code: %d)\n", result);
        }
        printf("\n");
    }

    return 0;
}