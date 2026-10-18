#include <stdio.h>
#include <string.h>
#include <limits.h>

#define CHAR_SET_SIZE 256

char find_first_repeated(const char *str)
{
    if (str == NULL)
    {
        return '\0';
    }

    int seen[CHAR_SET_SIZE] = {0};
    size_t i;

    for (i = 0; str[i] != '\0'; i++)
    {
        unsigned char c = (unsigned char)str[i];
        if (seen[c])
        {
            return (char)c;
        }
        seen[c] = 1;
    }

    return '\0';
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[1024];
    char result;

    printf("Enter a string: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    result = find_first_repeated(buffer);

    if (result != '\0')
    {
        printf("First repeated character: %c\n", result);
    }
    else
    {
        printf("No repeated character found\n");
    }

    return 0;
}