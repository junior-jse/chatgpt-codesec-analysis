#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define DATE_PATTERN "[0-9]{4}-[0-9]{2}-[0-9]{2}"

int extract_date_from_url(const char *url, int *year, int *month, int *day)
{
    regex_t regex;
    regmatch_t matches[1];
    char date_str[11];
    int ret;

    if (url == NULL || year == NULL || month == NULL || day == NULL)
    {
        return -1;
    }

    ret = regcomp(&regex, DATE_PATTERN, REG_EXTENDED);
    if (ret != 0)
    {
        return -1;
    }

    ret = regexec(&regex, url, 1, matches, 0);
    if (ret != 0)
    {
        regfree(&regex);
        return -1;
    }

    if (matches[0].rm_so < 0 || matches[0].rm_eo < 0)
    {
        regfree(&regex);
        return -1;
    }

    size_t len = (size_t)(matches[0].rm_eo - matches[0].rm_so);
    if (len != 10)
    {
        regfree(&regex);
        return -1;
    }

    strncpy(date_str, url + matches[0].rm_so, len);
    date_str[10] = '\0';

    if (sscanf(date_str, "%4d-%2d-%2d", year, month, day) != 3)
    {
        regfree(&regex);
        return -1;
    }

    if (*year < 1900 || *year > 2100 || *month < 1 || *month > 12 || *day < 1 || *day > 31)
    {
        regfree(&regex);
        return -1;
    }

    regfree(&regex);
    return 0;
}

int main(void)
{
    const char *url = "https://example.com/2024-01-15/article.html";
    int year = 0;
    int month = 0;
    int day = 0;

    if (extract_date_from_url(url, &year, &month, &day) == 0)
    {
        printf("Year: %d\n", year);
        printf("Month: %d\n", month);
        printf("Day: %d\n", day);
    }
    else
    {
        fprintf(stderr, "Failed to extract date from URL\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}