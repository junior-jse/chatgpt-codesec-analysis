#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int extract_date_from_url(const char *url, char *year, char *month, char *day) {
    regex_t regex;
    regmatch_t matches[4];
    int ret;

    if (url == NULL || year == NULL || month == NULL || day == NULL) {
        return -1;
    }

    ret = regcomp(&regex, "([0-9]{4})/([0-9]{2})/([0-9]{2})", REG_EXTENDED);
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

    size_t year_len = matches[1].rm_eo - matches[1].rm_so;
    size_t month_len = matches[2].rm_eo - matches[2].rm_so;
    size_t day_len = matches[3].rm_eo - matches[3].rm_so;

    if (year_len >= 5 || month_len >= 3 || day_len >= 3) {
        regfree(&regex);
        return -1;
    }

    memcpy(year, url + matches[1].rm_so, year_len);
    year[year_len] = '\0';

    memcpy(month, url + matches[2].rm_so, month_len);
    month[month_len] = '\0';

    memcpy(day, url + matches[3].rm_so, day_len);
    day[day_len] = '\0';

    regfree(&regex);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <url>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char year[5];
    char month[3];
    char day[3];

    if (extract_date_from_url(argv[1], year, month, day) == 0) {
        printf("Year: %s\n", year);
        printf("Month: %s\n", month);
        printf("Day: %s\n", day);
    } else {
        printf("No date found or extraction failed.\n");
    }

    return EXIT_SUCCESS;
}