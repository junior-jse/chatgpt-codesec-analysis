#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int extract_date_from_url(const char *url, char *year, size_t year_size, char *month, size_t month_size, char *day, size_t day_size) {
    regex_t regex;
    regmatch_t matches[4];
    int ret;

    if (url == NULL || year == NULL || month == NULL || day == NULL) {
        return -1;
    }

    if (year_size == 0 || month_size == 0 || day_size == 0) {
        return -1;
    }

    ret = regcomp(&regex, "/([0-9]{4})/([0-9]{2})/([0-9]{2})", REG_EXTENDED);
    if (ret != 0) {
        return -1;
    }

    ret = regexec(&regex, url, 4, matches, 0);
    if (ret != 0) {
        regfree(&regex);
        return -1;
    }

    size_t year_len = matches[1].rm_eo - matches[1].rm_so;
    size_t month_len = matches[2].rm_eo - matches[2].rm_so;
    size_t day_len = matches[3].rm_eo - matches[3].rm_so;

    if (year_len >= year_size || month_len >= month_size || day_len >= day_size) {
        regfree(&regex);
        return -1;
    }

    snprintf(year, year_size, "%.*s", (int)year_len, url + matches[1].rm_so);
    snprintf(month, month_size, "%.*s", (int)month_len, url + matches[2].rm_so);
    snprintf(day, day_size, "%.*s", (int)day_len, url + matches[3].rm_so);

    regfree(&regex);
    return 0;
}

int main(void) {
    const char *url = "https://example.com/archive/2023/10/25/article.html";
    char year[5];
    char month[3];
    char day[3];

    if (extract_date_from_url(url, year, sizeof(year), month, sizeof(month), day, sizeof(day)) == 0) {
        printf("Year: %s\n", year);
        printf("Month: %s\n", month);
        printf("Day: %s\n", day);
    } else {
        printf("Failed to extract date.\n");
    }

    return 0;
}