/* Repair history (previous round):
 * - Flawfinder strncpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <limits.h>
#include <errno.h>

#define MAX_URL_LEN 2048
#define MAX_MATCHES 4

typedef struct {
    int year;
    int month;
    int day;
    int valid;
} DateComponents;

int extract_date_from_url(const char *url, DateComponents *date) {
    regex_t regex;
    regmatch_t matches[MAX_MATCHES];
    const char *pattern = "([0-9]{4})[-/]([0-9]{1,2})[-/]([0-9]{1,2})";
    int ret;
    size_t url_len;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char year_str[5] = {0};
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char month_str[3] = {0};
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char day_str[3] = {0};
    int year_len, month_len, day_len;
    char *endptr;
    long year_val, month_val, day_val;
    
    if (url == NULL || date == NULL) {
        return -1;
    }
    
    url_len = strnlen(url, MAX_URL_LEN + 1);
    if (url_len == 0 || url_len > MAX_URL_LEN) {
        return -1;
    }
    
    date->valid = 0;
    
    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return -1;
    }
    
    ret = regexec(&regex, url, MAX_MATCHES, matches, 0);
    if (ret != 0) {
        regfree(&regex);
        return -1;
    }
    
    if (matches[1].rm_so < 0 || matches[1].rm_eo < 0 ||
        matches[2].rm_so < 0 || matches[2].rm_eo < 0 ||
        matches[3].rm_so < 0 || matches[3].rm_eo < 0 ||
        matches[1].rm_so >= matches[1].rm_eo ||
        matches[2].rm_so >= matches[2].rm_eo ||
        matches[3].rm_so >= matches[3].rm_eo ||
        (size_t)matches[1].rm_eo > url_len ||
        (size_t)matches[2].rm_eo > url_len ||
        (size_t)matches[3].rm_eo > url_len) {
        regfree(&regex);
        return -1;
    }
    
    year_len = matches[1].rm_eo - matches[1].rm_so;
    month_len = matches[2].rm_eo - matches[2].rm_so;
    day_len = matches[3].rm_eo - matches[3].rm_so;
    
    if (year_len != 4 || month_len < 1 || month_len > 2 || day_len < 1 || day_len > 2) {
        regfree(&regex);
        return -1;
    }
    
    if ((size_t)year_len >= sizeof(year_str) || 
        (size_t)month_len >= sizeof(month_str) || 
        (size_t)day_len >= sizeof(day_str)) {
        regfree(&regex);
        return -1;
    }
    
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(year_str, url + matches[1].rm_so, (size_t)year_len);
    year_str[year_len] = '\0';
    
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(month_str, url + matches[2].rm_so, (size_t)month_len);
    month_str[month_len] = '\0';
    
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(day_str, url + matches[3].rm_so, (size_t)day_len);
    day_str[day_len] = '\0';
    
    errno = 0;
    year_val = strtol(year_str, &endptr, 10);
    if (errno != 0 || *endptr != '\0' || year_val < 1900 || year_val > 2100) {
        regfree(&regex);
        return -1;
    }
    
    errno = 0;
    month_val = strtol(month_str, &endptr, 10);
    if (errno != 0 || *endptr != '\0' || month_val < 1 || month_val > 12) {
        regfree(&regex);
        return -1;
    }
    
    errno = 0;
    day_val = strtol(day_str, &endptr, 10);
    if (errno != 0 || *endptr != '\0' || day_val < 1 || day_val > 31) {
        regfree(&regex);
        return -1;
    }
    
    date->year = (int)year_val;
    date->month = (int)month_val;
    date->day = (int)day_val;
    date->valid = 1;
    
    regfree(&regex);
    return 0;
}

int main(void) {
    const char *test_urls[] = {
        "https://example.com/2024/01/15/article",
        "https://example.com/blog/2023-12-25/post",
        "https://example.com/invalid/path",
        "https://example.com/2024/13/45/invalid"
    };
    
    size_t num_urls = sizeof(test_urls) / sizeof(test_urls[0]);
    size_t i;
    DateComponents date;
    
    for (i = 0; i < num_urls; i++) {
        memset(&date, 0, sizeof(date));
        
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