#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char **split_string(const char *input, const char *delimiters, int *count) {
    if (!input || !delimiters || !count) {
        return NULL;
    }

    regex_t regex;
    char pattern[512];
    snprintf(pattern, sizeof(pattern), "[%s]+", delimiters);

    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    char **tokens = NULL;
    int token_count = 0;
    const char *current = input;
    regmatch_t match;

    while (*current) {
        if (regexec(&regex, current, 1, &match, 0) == 0) {
            if (match.rm_so > 0) {
                tokens = realloc(tokens, (token_count + 1) * sizeof(char *));
                if (!tokens) {
                    regfree(&regex);
                    if (count) *count = 0;
                    return NULL;
                }
                tokens[token_count] = strndup(current, match.rm_so);
                if (!tokens[token_count]) {
                    for (int i = 0; i < token_count; i++) {
                        free(tokens[i]);
                    }
                    free(tokens);
                    regfree(&regex);
                    if (count) *count = 0;
                    return NULL;
                }
                token_count++;
            }
            current += match.rm_eo;
        } else {
            size_t remaining = strlen(current);
            if (remaining > 0) {
                tokens = realloc(tokens, (token_count + 1) * sizeof(char *));
                if (!tokens) {
                    regfree(&regex);
                    if (count) *count = 0;
                    return NULL;
                }
                tokens[token_count] = strdup(current);
                if (!tokens[token_count]) {
                    for (int i = 0; i < token_count; i++) {
                        free(tokens[i]);
                    }
                    free(tokens);
                    regfree(&regex);
                    if (count) *count = 0;
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

void free_tokens(char **tokens, int count) {
    if (!tokens) return;
    for (int i = 0; i < count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

int main(int argc, char *argv[]) {
    const char *input = "Hello,world;this:is|a test,string;with:multiple delimiters";
    const char *delimiters = ",;:| ";
    int count = 0;

    char **tokens = split_string(input, delimiters, &count);

    if (!tokens) {
        fprintf(stderr, "Failed to split string\n");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        printf("Token %d: %s\n", i, tokens[i]);
    }

    free_tokens(tokens, count);
    return 0;
}