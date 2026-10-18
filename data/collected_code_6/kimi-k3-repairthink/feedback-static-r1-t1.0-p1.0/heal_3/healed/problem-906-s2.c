#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>

#define MAX_URL_LEN 2048
#define MAX_MATCHES 4
#define YEAR_BUF_SIZE 5
#define MONTH_BUF_SIZE 3
#define DAY_BUF_SIZE 3

typedef struct {
    int year;
    int month;
    int day;
    int valid;
} DateComponents;

static int parse_bounded_int(const char *str, int min, int max, int *out)
{
    char *endptr = NULL;
    long value;

    if (str == NULL || out == NULL || str[0] == '\0') {
        return -1;
    }

    errno = 0;
    value = strtol(str, &endptr, 10);

    if (errno != 0 || endptr == str || *endptr != '\0') {
        return -1;
    }

    if (value < min || value > max) {
        return -1;
    }

    *out = (int)value;
    return 0;
}

static int copy_bounded(char *dest, size_t dest_size, const char *src, size_t src_len)
{
    int written;

    if (dest == NULL || src == NULL || dest_size == 0) {
        return -1;
    }

    if (src_len == 0 || src_len >= dest_size) {
        return -1;
    }

    written = snprintf(dest, dest_size, "%.*s", (int)src_len, src);
    if (written < 0 || (size_t)written >= dest_size) {
        dest[0] = '\0';
        return -1;
    }

    return 0;
}

int extract_date_from_url(const char *url, DateComponents *date)
{
    regex_t regex;
    regmatch_t matches[MAX_MATCHES];
    const char *pattern = "([0-9]{4})[-/]([0-9]{1,2})[-/]([0-9]{1,2})";
    char *year_str = NULL;
    char *month_str = NULL;
    char *day_str = NULL;
    size_t year_len;
    size_t month_len;
    size_t day_len;
    int ret;
    int result = -1;
    int regex_compiled = 0;

    if (url == NULL || date == NULL) {
        return -1;
    }

    date->year = 0;
    date->month = 0;
    date->day = 0;
    date->valid = 0;

    if (strnlen(url, MAX_URL_LEN) >= MAX_URL_LEN) {
        return -1;
    }

    year_str = calloc(YEAR_BUF_SIZE, sizeof(*year_str));
    month_str = calloc(MONTH_BUF_SIZE, sizeof(*month_str));
    day_str = calloc(DAY_BUF_SIZE, sizeof(*day_str));

    if (year_str == NULL || month_str == NULL || day_str == NULL) {
        goto cleanup;
    }

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        goto cleanup;
    }
    regex_compiled = 1;

    ret = regexec(&regex, url, MAX_MATCHES, matches, 0);
    if (ret != 0) {
        goto cleanup;
    }

    if (matches[1].rm_so < 0 || matches[1].rm_eo < matches[1].rm_so ||
        matches[2].rm_so < 0 || matches[2].rm_eo < matches[2].rm_so ||
        matches[3].rm_so < 0 || matches[3].rm_eo < matches[3].rm_so) {
        goto cleanup;
    }

    year_len = (size_t)(matches[1].rm_eo - matches[1].rm_so);
    month_len = (size_t)(matches[2].rm_eo - matches[2].rm_so);
    day_len = (size_t)(matches[3].rm_eo - matches[3].rm_so);

    if (year_len >= YEAR_BUF_SIZE ||
        month_len >= MONTH_BUF_SIZE ||
        day_len >= DAY_BUF_SIZE) {
        goto cleanup;
    }

    if (copy_bounded(year_str, YEAR_BUF_SIZE, url + matches[1].rm_so, year_len) != 0 ||
        copy_bounded(month_str, MONTH_BUF_SIZE, url + matches[2].rm_so, month_len) != 0 ||
        copy_bounded(day_str, DAY_BUF_SIZE, url + matches[3].rm_so, day_len) != 0) {
        goto cleanup;
    }

    if (parse_bounded_int(year_str, 1900, 2100, &date->year) != 0 ||
        parse_bounded_int(month_str, 1, 12, &date->month) != 0 ||
        parse_bounded_int(day_str, 1, 31, &date->day) != 0) {
        goto cleanup;
    }

    date->valid = 1;
    result = 0;

cleanup:
    if (regex_compiled) {
        regfree(&regex);
    }
    free(year_str);
    free(month_str);
    free(day_str);
    return result;
}

int main(void)
{
    const char *test_urls[] = {
        "https://example.com/2024/01/15/article",
        "https://example.com/blog/2023-12-25/post",
        "https://example.com/invalid/path",
        "https://example.com/2024/13/45/invalid"
    };

    size_t num_urls = sizeof(test_urls) / sizeof(test_urls[0]);

    for (size_t i = 0; i < num_urls; i++) {
        DateComponents date = {0};

        if (extract_date_from_url(test_urls[i], &date) == 0 && date.valid) {
            printf("URL: %s\n", test_urls[i]);
            printf("Year: %d, Month: %d, Day: %d\n\n",
                   date.year, date.month, date.day);
        } else {
            printf("URL: %s\n", test_urls[i]);
            printf("No valid date found\n\n");
        }
    }

    return 0;
}