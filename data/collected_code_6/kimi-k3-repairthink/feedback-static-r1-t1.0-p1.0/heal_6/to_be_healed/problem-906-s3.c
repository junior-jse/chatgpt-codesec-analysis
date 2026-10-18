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

#define YEAR_STR_SIZE   5
#define MONTH_STR_SIZE  3
#define DAY_STR_SIZE    3
#define YEAR_DIGITS     4
#define MONTH_DIGITS    2
#define DAY_DIGITS      2

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

static int copy_match_safe(char *dest, size_t dest_size, const char *src, regmatch_t match) {
    size_t len;
    size_t src_len;
    int written;

    if (dest == NULL || src == NULL || dest_size == 0) {
        return -1;
    }

    if (match.rm_so < 0 || match.rm_eo <= match.rm_so) {
        return -1;
    }

    len = (size_t)(match.rm_eo - match.rm_so);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    src_len = strlen(src);

    if ((size_t)match.rm_so >= src_len || len > src_len - (size_t)match.rm_so) {
        return -1;
    }

    /* Possible weaknesses found:
     *  Assuming that condition 'len>=dest_size' is not redundant
     */
    if (len >= dest_size) {
        return -1;
    }

    written = snprintf(dest, dest_size, "%.*s", (int)len, src + match.rm_so);
    /* Possible weaknesses found:
     *  Condition '(unsigned long)written>=dest_size' is always false
     *  Assuming that condition '(unsigned long)written!=len' is not redundant
     *  Condition '(unsigned long)written>=dest_size' is always false [knownConditionTrueFalse]
     */
    if (written < 0 || (size_t)written != len || (size_t)written >= dest_size) {
        dest[0] = '\0';
        return -1;
    }

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
    char year_str[YEAR_STR_SIZE];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char month_str[MONTH_STR_SIZE];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char day_str[DAY_STR_SIZE];
    DateComponents parsed = {0, 0, 0};

    if (url == NULL || date == NULL) {
        return -1;
    }

    if (YEAR_STR_SIZE < YEAR_DIGITS + 1 ||
        MONTH_STR_SIZE < MONTH_DIGITS + 1 ||
        DAY_STR_SIZE < DAY_DIGITS + 1) {
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

    if (matches[1].rm_so < 0 || matches[2].rm_so < 0 || matches[3].rm_so < 0) {
        regfree(&regex);
        return -1;
    }

    if ((matches[1].rm_eo - matches[1].rm_so) != YEAR_DIGITS ||
        (matches[2].rm_eo - matches[2].rm_so) != MONTH_DIGITS ||
        (matches[3].rm_eo - matches[3].rm_so) != DAY_DIGITS) {
        regfree(&regex);
        return -1;
    }

    if (copy_match_safe(year_str, sizeof(year_str), url, matches[1]) != 0 ||
        copy_match_safe(month_str, sizeof(month_str), url, matches[2]) != 0 ||
        copy_match_safe(day_str, sizeof(day_str), url, matches[3]) != 0) {
        regfree(&regex);
        return -1;
    }

    if (parse_int_safe(year_str, &parsed.year) != 0 ||
        parse_int_safe(month_str, &parsed.month) != 0 ||
        parse_int_safe(day_str, &parsed.day) != 0) {
        regfree(&regex);
        return -1;
    }

    if (parsed.year < 1900 || parsed.year > 2100 ||
        parsed.month < 1 || parsed.month > 12 ||
        parsed.day < 1 || parsed.day > 31) {
        regfree(&regex);
        return -1;
    }

    regfree(&regex);

    date->year = parsed.year;
    date->month = parsed.month;
    date->day = parsed.day;

    return 0;
}

int main(void) {
    const char *test_urls[] = {
        "https://example.com/2024/01/15/article",
        "https://example.com/2023/12/25/news",
        "https://example.com/invalid/path",
        "https://example.com/20241/01/15/overflow",
        "https://example.com/2024/13/40/badmonth",
    };
    size_t num_urls = sizeof(test_urls) / sizeof(test_urls[0]);
    size_t i;

    for (i = 0; i < num_urls; i++) {
        DateComponents date = {0, 0, 0};
        int result = extract_date_from_url(test_urls[i], &date);

        printf("URL: %s\n", test_urls[i]);
        if (result == 0) {
            printf("Year: %d, Month: %d, Day: %d\n\n", date.year, date.month, date.day);
        } else {
            printf("Failed to extract date\n\n");
        }
    }

    return 0;
}