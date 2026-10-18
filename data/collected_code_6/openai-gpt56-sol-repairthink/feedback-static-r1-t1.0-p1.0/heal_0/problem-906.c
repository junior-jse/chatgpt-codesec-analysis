#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int copy_match(const char *source, regmatch_t match, char *destination,
                      size_t destination_size)
{
    size_t length;

    if (source == NULL || destination == NULL || destination_size == 0 ||
        match.rm_so < 0 || match.rm_eo < match.rm_so) {
        return -1;
    }

    length = (size_t)(match.rm_eo - match.rm_so);
    if (length >= destination_size) {
        return -1;
    }

    memcpy(destination, source + match.rm_so, length);
    destination[length] = '\0';

    return 0;
}

static int extract_date_from_url(const char *url, int *year, int *month, int *day)
{
    static const char pattern[] =
        "([^0-9]|^)([0-9]{4})[-/]([0-9]{2})[-/]([0-9]{2})([^0-9]|$)";
    regex_t regex;
    regmatch_t matches[6];
    char year_text[5];
    char month_text[3];
    char day_text[3];
    char *end;
    long value;
    int result;

    if (url == NULL || year == NULL || month == NULL || day == NULL) {
        return -1;
    }

    result = regcomp(&regex, pattern, REG_EXTENDED);
    if (result != 0) {
        return -1;
    }

    result = regexec(&regex, url, 6, matches, 0);
    if (result != 0) {
        regfree(&regex);
        return result == REG_NOMATCH ? 1 : -1;
    }

    if (copy_match(url, matches[2], year_text, sizeof(year_text)) != 0 ||
        copy_match(url, matches[3], month_text, sizeof(month_text)) != 0 ||
        copy_match(url, matches[4], day_text, sizeof(day_text)) != 0) {
        regfree(&regex);
        return -1;
    }

    regfree(&regex);

    value = strtol(year_text, &end, 10);
    if (*end != '\0' || value < 1 || value > 9999) {
        return 1;
    }
    *year = (int)value;

    value = strtol(month_text, &end, 10);
    if (*end != '\0' || value < 1 || value > 12) {
        return 1;
    }
    *month = (int)value;

    value = strtol(day_text, &end, 10);
    if (*end != '\0' || value < 1 || value > 31) {
        return 1;
    }
    *day = (int)value;

    return 0;
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
    if (result != 0) {
        fprintf(stderr, "Unable to extract a valid date from the URL\n");
        return EXIT_FAILURE;
    }

    printf("Year: %04d\nMonth: %02d\nDate: %02d\n", year, month, day);

    return EXIT_SUCCESS;
}