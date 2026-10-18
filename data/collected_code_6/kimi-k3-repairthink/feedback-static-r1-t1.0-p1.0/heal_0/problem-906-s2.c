#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

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
    
    if (url == NULL || date == NULL) {
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
    
    if (matches[1].rm_so == -1 || matches[2].rm_so == -1 || matches[3].rm_so == -1) {
        regfree(&regex);
        return -1;
    }
    
    char year_str[5] = {0};
    char month_str[3] = {0};
    char day_str[3] = {0};
    
    int year_len = matches[1].rm_eo - matches[1].rm_so;
    int month_len = matches[2].rm_eo - matches[2].rm_so;
    int day_len = matches[3].rm_eo - matches[3].rm_so;
    
    if (year_len > 4 || month_len > 2 || day_len > 2) {
        regfree(&regex);
        return -1;
    }
    
    strncpy(year_str, url + matches[1].rm_so, year_len);
    strncpy(month_str, url + matches[2].rm_so, month_len);
    strncpy(day_str, url + matches[3].rm_so, day_len);
    
    date->year = atoi(year_str);
    date->month = atoi(month_str);
    date->day = atoi(day_str);
    
    if (date->year < 1900 || date->year > 2100 ||
        date->month < 1 || date->month > 12 ||
        date->day < 1 || date->day > 31) {
        regfree(&regex);
        return -1;
    }
    
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