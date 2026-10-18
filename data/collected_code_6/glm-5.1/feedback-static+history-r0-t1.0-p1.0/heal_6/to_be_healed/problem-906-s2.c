#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int extract_date_from_url(const char *url, char *year, char *month, char *day, size_t year_size, size_t month_size, size_t day_size) {
    regex_t regex;
    regmatch_t matches[4];
    int ret;

    ret = regcomp(&regex, "/([0-9]{4})/([0-9]{2})/([0-9]{2})", REG_EXTENDED);
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

int main(int argc, char const * const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <url>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char year[5 + 1];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char month[3 + 1];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char day[3 + 1];

    if (extract_date_from_url(argv[1], year, month, day, sizeof(year), sizeof(month), sizeof(day)) == 0) {
        printf("Year: %s\n", year);
        printf("Month: %s\n", month);
        printf("Day: %s\n", day);
    } else {
        fprintf(stderr, "No date pattern found in URL.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}