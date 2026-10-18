#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>

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

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(destination, source + match.rm_so, length);
    destination[length] = '\0';

    return 0;
}

static int extract_date_from_url(const char *url, int *year, int *month, int *day)
{
    const char *pattern =
        "(^|[^0-9])([0-9]{4})[-/]([0-9]{2})[-/]([0-9]{2})([^0-9]|$)";
    regex_t regex;
    regmatch_t matches[6];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char year_text[5];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char month_text[3];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char day_text[3];
    char *end = NULL;
    long values[3];
    int result;
    /* Possible weaknesses found:
     *  The scope of the variable 'i' can be reduced. [variableScope]
     */
    size_t i;

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

    {
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        const char *texts[3] = {year_text, month_text, day_text};

        for (i = 0; i < 3; ++i) {
            errno = 0;
            end = NULL;
            values[i] = strtol(texts[i], &end, 10);
            if (errno != 0 || end == texts[i] || *end != '\0') {
                return -1;
            }
        }
    }

    if (values[0] < 1 || values[0] > 9999 ||
        values[1] < 1 || values[1] > 12 ||
        values[2] < 1 || values[2] > 31) {
        return 1;
    }

    *year = (int)values[0];
    *month = (int)values[1];
    *day = (int)values[2];

    return 0;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
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
        fprintf(stderr, "No valid date found\n");
        return EXIT_FAILURE;
    }

    if (result != 0) {
        fprintf(stderr, "Failed to extract date\n");
        return EXIT_FAILURE;
    }

    if (printf("Year: %04d\nMonth: %02d\nDate: %02d\n",
               year, month, day) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}