#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define DATE_PATTERN "[0-9]{4}-[0-9]{2}-[0-9]{2}"
#define DATE_STR_SIZE 16
#define DATE_EXPECTED_LEN 10

static int days_in_month(int year, int month)
{
    static const int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int d;

    if (month < 1 || month > 12)
    {
        return 0;
    }

    d = days[month - 1];
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
    {
        d = 29;
    }

    return d;
}

int extract_date_from_url(const char *url, int *year, int *month, int *day)
{
    regex_t regex;
    regmatch_t matches[1];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char date_str[DATE_STR_SIZE];
    size_t len;
    int ret;
    int result = -1;

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
        goto cleanup;
    }

    if (matches[0].rm_so < 0 || matches[0].rm_eo < 0 ||
        matches[0].rm_eo <= matches[0].rm_so)
    {
        goto cleanup;
    }

    len = (size_t)(matches[0].rm_eo - matches[0].rm_so);
    if (len != DATE_EXPECTED_LEN || len >= sizeof(date_str))
    {
        goto cleanup;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(date_str, url + matches[0].rm_so, len);
    date_str[len] = '\0';

    if (sscanf(date_str, "%4d-%2d-%2d", year, month, day) != 3)
    {
        goto cleanup;
    }

    if (*year < 1900 || *year > 2100 || *month < 1 || *month > 12 ||
        *day < 1 || *day > days_in_month(*year, *month))
    {
        goto cleanup;
    }

    result = 0;

cleanup:
    regfree(&regex);
    return result;
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