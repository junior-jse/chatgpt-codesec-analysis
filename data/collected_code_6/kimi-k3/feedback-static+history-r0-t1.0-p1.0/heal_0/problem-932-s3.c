#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int removeDuplicateWords(char *words[], int count)
{
    if (words == NULL || count <= 0)
    {
        return 0;
    }

    int uniqueCount = 0;

    for (int i = 0; i < count; i++)
    {
        if (words[i] == NULL)
        {
            continue;
        }

        int isDuplicate = 0;

        for (int j = 0; j < uniqueCount; j++)
        {
            if (strcmp(words[i], words[j]) == 0)
            {
                isDuplicate = 1;
                break;
            }
        }

        if (!isDuplicate)
        {
            words[uniqueCount] = words[i];
            uniqueCount++;
        }
    }

    return uniqueCount;
}

int main(void)
{
    char *words[] = {
        "apple",
        "banana",
        "orange",
        "apple",
        "grape",
        "banana",
        "melon"
    };

    int count = (int)(sizeof(words) / sizeof(words[0]));
    int uniqueCount = removeDuplicateWords(words, count);

    if (uniqueCount < 0)
    {
        fprintf(stderr, "Error removing duplicate words\n");
        return EXIT_FAILURE;
    }

    printf("Unique words:\n");

    for (int i = 0; i < uniqueCount; i++)
    {
        if (printf("%s\n", words[i]) < 0)
        {
            fprintf(stderr, "Error printing output\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}