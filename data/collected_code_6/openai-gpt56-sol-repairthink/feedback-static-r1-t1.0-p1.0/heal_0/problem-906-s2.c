#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>
#include <limits.h>

static int parse_component(const char *text, size_t length, int *value)
{
    char buffer[5];
    char *end = NULL;
    long result;

    if (text == NULL || value == NULL || length == 0 || length >= sizeof(buffer)) {
        return -1;
    }

    memcpy(buffer, text, length);
    buffer[length] = '\0';

    errno = 0;
    result = strtol(buffer, &end, 10);
    if (errno != 0 || end == buffer || *end != '\0' ||
        result < 0 || result > INT_MAX) {
        return -1;
    }

    *value = (int)result;
    return 0;
}

static int is_leap_year(int year)
{
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

static int is_valid_date(int year, int month, int day)
{
    static const int days_per_month[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };
    int maximum_day;

    if (year < 1 || month < 1 || month > 12 || day < 1) {
        return 0;
    }

    maximum_day = days_per_month[month - 1];
    if (month == 2 && is_leap_year(year)) {
        maximum_day = 29;
    }

    return day <= maximum_day;
}

static int extract_date_from_url(const char *url, int *year, int *month, int *day)
{
    static const char pattern[] =
        "(^|[^0-9])([0-9]{4})[-/]([0-9]{2})[-/]([0-9]{2})([^0-9]|$)";
    regex_t regex;
    regmatch_t matches[6];
    int compile_result;
    int execute_result;
    int parsed_year;
    int parsed_month;
    int parsed_day;
    int status = -1;

    if (url == NULL || year == NULL || month == NULL || day == NULL) {
        return -1;
    }

    compile_result = regcomp(&regex, pattern, REG_EXTENDED);
    if (compile_result != 0) {
        return -1;
    }

    execute_result = regexec(&regex, url, 6, matches, 0);
    if (execute_result != 0) {
        regfree(&regex);
        return execute_result == REG_NOMATCH ? 1 : -1;
    }

    if (matches[2].rm_so < 0 || matches[3].rm_so < 0 || matches[4].rm_so < 0) {
        regfree(&regex);
        return -1;
    }

    if (parse_component(url + matches[2].rm_so,
                        (size_t)(matches[2].rm_eo - matches[2].rm_so),
                        &parsed_year) == 0 &&
        parse_component(url + matches[3].rm_so,
                        (size_t)(matches[3].rm_eo - matches[3].rm_so),
                        &parsed_month) == 0 &&
        parse_component(url + matches[4].rm_so,
                        (size_t)(matches[4].rm_eo - matches[4].rm_so),
                        &parsed_day) == 0 &&
        is_valid_date(parsed_year, parsed_month, parsed_day)) {
        *year = parsed_year;
        *month = parsed_month;
        *day = parsed_day;
        status = 0;
    }

    regfree(&regex);
    return status;
}

int main(int argc, char *argv[])
{
    int year;
    int month;
    int day;
    int result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <url>\n", argv[0]);
        return EXIT_FAILURE;
    }

    result = extract_date_from_url(argv[1], &year, &month, &day);
    if (result == 1) {
        fprintf(stderr, "No date found in URL\n");
        return EXIT_FAILURE;
    }

    if (result != 0) {
        fprintf(stderr, "Failed to extract a valid date\n");
        return EXIT_FAILURE;
    }

    if (printf("Year: %04d\nMonth: %02d\nDate: %02d\n",
               year, month, day) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}