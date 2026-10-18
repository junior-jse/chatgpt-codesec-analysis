#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

static size_t safe_strlen(const char *s, size_t max_len) {
    if (!s) return 0;
    size_t len = 0;
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

static char *safe_strndup(const char *s, size_t len) {
    if (!s || len == 0) return NULL;
    char *dup = malloc(len + 1);
    if (!dup) return NULL;
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(dup, s, len);
    dup[len] = '\0';
    return dup;
}

char **split_string(const char *input, size_t input_len, const char *delimiters, size_t delim_len, int *count) {
    if (!input || !delimiters || !count || delim_len == 0 || input_len == 0) {
        if (count) *count = 0;
        return NULL;
    }

    regex_t regex;
    size_t pattern_size = delim_len + 5;
    char *pattern = malloc(pattern_size);
    if (!pattern) {
        *count = 0;
        return NULL;
    }

    int written = snprintf(pattern, pattern_size, "[%s]+", delimiters);
    if (written < 0 || (size_t)written >= pattern_size) {
        free(pattern);
        *count = 0;
        return NULL;
    }

    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    free(pattern);
    if (ret != 0) {
        *count = 0;
        return NULL;
    }

    char **tokens = NULL;
    int token_count = 0;
    const char *current = input;
    size_t remaining_len = input_len;
    regmatch_t match;

    /* Possible weaknesses found:
     *  Assuming that condition 'remaining_len>0' is not redundant
     */
    while (remaining_len > 0) {
        if (regexec(&regex, current, 1, &match, 0) == 0) {
            if (match.rm_so > 0) {
                if ((size_t)match.rm_so > remaining_len) {
                    /* Possible weaknesses found:
                     *  previous implicit declaration is here
                     *  implicit declaration of function 'free_tokens' [-Wimplicit-function-declaration]
                     *  call to undeclared function 'free_tokens'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
                     */
                    free_tokens(tokens, token_count);
                    regfree(&regex);
                    *count = 0;
                    return NULL;
                }
                char **new_tokens = realloc(tokens, (size_t)(token_count + 1) * sizeof(char *));
                if (!new_tokens) {
                    /* Possible weaknesses found:
                     *  call to undeclared function 'free_tokens'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
                     */
                    free_tokens(tokens, token_count);
                    regfree(&regex);
                    *count = 0;
                    return NULL;
                }
                tokens = new_tokens;
                tokens[token_count] = safe_strndup(current, (size_t)match.rm_so);
                if (!tokens[token_count]) {
                    /* Possible weaknesses found:
                     *  call to undeclared function 'free_tokens'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
                     */
                    free_tokens(tokens, token_count);
                    regfree(&regex);
                    *count = 0;
                    return NULL;
                }
                token_count++;
            }
            size_t advance = (size_t)match.rm_eo;
            if (advance > remaining_len) {
                advance = remaining_len;
            }
            current += advance;
            remaining_len -= advance;
        } else {
            /* Possible weaknesses found:
             *  Condition 'remaining_len>0' is always true
             *  Condition 'remaining_len>0' is always true [knownConditionTrueFalse]
             */
            if (remaining_len > 0) {
                char **new_tokens = realloc(tokens, (size_t)(token_count + 1) * sizeof(char *));
                if (!new_tokens) {
                    /* Possible weaknesses found:
                     *  call to undeclared function 'free_tokens'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
                     */
                    free_tokens(tokens, token_count);
                    regfree(&regex);
                    *count = 0;
                    return NULL;
                }
                tokens = new_tokens;
                tokens[token_count] = safe_strndup(current, remaining_len);
                if (!tokens[token_count]) {
                    /* Possible weaknesses found:
                     *  call to undeclared function 'free_tokens'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
                     */
                    free_tokens(tokens, token_count);
                    regfree(&regex);
                    *count = 0;
                    return NULL;
                }
                token_count++;
            }
            break;
        }
    }

    regfree(&regex);
    *count = token_count;
    return tokens;
}

/* Possible weaknesses found:
 *  conflicting types for 'free_tokens'
 */
void free_tokens(char **tokens, int count) {
    if (!tokens) return;
    for (int i = 0; i < count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    const char *input = "Hello,world;this:is|a test,string;with:multiple delimiters";
    size_t input_len = safe_strlen(input, 4096);
    const char *delimiters = ",;:| ";
    size_t delim_len = safe_strlen(delimiters, 256);
    int count = 0;

    char **tokens = split_string(input, input_len, delimiters, delim_len, &count);

    if (!tokens && count == 0) {
        fprintf(stderr, "Failed to split string or empty input\n");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        printf("Token %d: %s\n", i, tokens[i]);
    }

    free_tokens(tokens, count);
    return 0;
}