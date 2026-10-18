#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

static int parse_int(const char *str, int len, int *out) {
    if (str == NULL || len <= 0) return 0;
    int val = 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit((unsigned char)str[i])) return 0;
        val = val * 10 + (str[i] - '0');
    }
    *out = val;
    return 1;
}

int extract_date_from_url(const char *url, int *year, int *month, int *day) {
    if (url == NULL || year == NULL || month == NULL || day == NULL) {
        return -1;
    }

    regex_t regex;
    regmatch_t matches[4];
    const char *pattern = "([0-9]{4})[-/]([0-9]{1,2})[-/]([0-9]{1,2})";
    int ret;

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

    int y_len = matches[1].rm_eo - matches[1].rm_so;
    int m_len = matches[2].rm_eo - matches[2].rm_so;
    int d_len = matches[3].rm_eo - matches[3].rm_so;

    if (y_len > 4 || y_len <= 0 || m_len > 2 || m_len <= 0 || d_len > 2 || d_len <= 0) {
        regfree(&regex);
        return -1;
    }

    int y_val, m_val, d_val;

    if (!parse_int(url + matches[1].rm_so, y_len, &y_val) ||
        !parse_int(url + matches[2].rm_so, m_len, &m_val) ||
        !parse_int(url + matches[3].rm_so, d_len, &d_val)) {
        regfree(&regex);
        return -1;
    }

    if (y_val < 1900 || y_val > 2100) {
        regfree(&regex);
        return -1;
    }
    if (m_val < 1 || m_val > 12) {
        regfree(&regex);
        return -1;
    }
    if (d_val < 1 || d_val > 31) {
        regfree(&regex);
        return -1;
    }

    if (m_val == 2) {
        int is_leap = (y_val % 4 == 0 && y_val % 100 != 0) || (y_val % 400 == 0);
        int max_day = is_leap ? 29 : 28;
        if (d_val > max_day) {
            regfree(&regex);
            return -1;
        }
    } else if (m_val == 4 || m_val == 6 || m_val == 9 || m_val == 11) {
        if (d_val > 30) {
            regfree(&regex);
            return -1;
        }
    }

    *year = y_val;
    *month = m_val;
    *day = d_val;

    regfree(&regex);
    return 0;
}

int main(void) {
    const char *test_urls[] = {
        "https://example.com/2023/12/25/article",
        "http://test.org/2024-02-29/news",
        "https://invalid.com/2023-13-01/test",
        "https://invalid.com/2023-02-30/test",
        "https://no-date.com/test",
        NULL
    };

    for (int i = 0; test_urls[i] != NULL; i++) {
        int year = 0, month = 0, day = 0;
        int result = extract_date_from_url(test_urls[i], &year, &month, &day);
        
        if (result == 0) {
            printf("URL: %s\nDate: %04d-%02d-%02d\n\n", test_urls[i], year, month, day);
        } else {
            printf("URL: %s\nNo valid date found\n\n", test_urls[i]);
        }
    }

    return 0;
}