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

static int safe_atoi(const char *str, int *result) {
    char *endptr;
    long val;

    if (str == NULL || result == NULL) {
        return -1;
    }

    errno = 0;
    val = strtol(str, &endptr, 10);

    if (errno != 0 || endptr == str || *endptr != '\0') {
        return -1;
    }

    if (val < INT_MIN || val > INT_MAX) {
        return -1;
    }

    *result = (int)val;
    return 0;
}

int extract_date_from_url(const char *url, DateComponents *date) {
    regex_t regex;
    regmatch_t matches[4];
    const char *pattern = "/([0-9]{4})/([0-9]{2})/([0-9]{2})/";
    int ret;
    char year_str[5] = {0};
    char month_str[3] = {0};
    char day_str[3] = {0};
    size_t year_len, month_len, day_len;

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

    if (matches[1].rm_so < 0 || matches[2].rm_so < 0 || matches[3].rm_so < 0) {
        regfree(&regex);
        return -1;
    }

    year_len = (size_t)(matches[1].rm_eo - matches[1].rm_so);
    month_len = (size_t)(matches[2].rm_eo - matches[2].rm_so);
    day_len = (size_t)(matches[3].rm_eo - matches[3].rm_so);

    if (year_len != 4 || month_len != 2 || day_len != 2) {
        regfree(&regex);
        return -1;
    }

    if (year_len >= sizeof(year_str) || month_len >= sizeof(month_str) || day_len >= sizeof(day_str)) {
        regfree(&regex);
        return -1;
    }

    memcpy(year_str, url + matches[1].rm_so, year_len);
    year_str[year_len] = '\0';
    
    memcpy(month_str, url + matches[2].rm_so, month_len);
    month_str[month_len] = '\0';
    
    memcpy(day_str, url + matches[3].rm_so, day_len);
    day_str[day_len] = '\0';

    if (safe_atoi(year_str, &date->year) != 0 ||
        safe_atoi(month_str, &date->month) != 0 ||
        safe_atoi(day_str, &date->day) != 0) {
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