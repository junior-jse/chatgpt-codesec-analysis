#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CHAR_SET_SIZE 256
#define INITIAL_CAPACITY 128

char find_first_repeated(const char *str)
{
    int seen[CHAR_SET_SIZE] = {0};
    size_t i;

    if (str == NULL)
    {
        return '\0';
    }

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

static char *read_input_line(FILE *stream)
{
    size_t capacity = INITIAL_CAPACITY;
    size_t length = 0;
    char *buffer;
    int ch;

    if (stream == NULL)
    {
        return NULL;
    }

    buffer = (char *)malloc(capacity);
    if (buffer == NULL)
    {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder fgetc: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((ch = fgetc(stream)) != EOF && ch != '\n')
    {
        if (length + 1 >= capacity)
        {
            size_t new_capacity;
            char *temp;

            if (capacity > SIZE_MAX / 2)
            {
                free(buffer);
                return NULL;
            }

            new_capacity = capacity * 2;
            temp = (char *)realloc(buffer, new_capacity);
            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }

            buffer = temp;
            capacity = new_capacity;
        }

        buffer[length] = (char)ch;
        length++;
    }

    if (ferror(stream))
    {
        free(buffer);
        return NULL;
    }

    if (ch == EOF && length == 0)
    {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

int main(void)
{
    char *buffer;
    char result;

    printf("Enter a string: ");
    fflush(stdout);

    buffer = read_input_line(stdin);
    if (buffer == NULL)
    {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    result = find_first_repeated(buffer);

    if (result != '\0')
    {
        printf("First repeated character: %c\n", result);
    }
    else
    {
        printf("No repeated character found\n");
    }

    free(buffer);
    return 0;
}