#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

bool are_all_characters_unique(const char *str)
{
    if (str == NULL)
    {
        return false;
    }

    uint32_t seen[8] = {0};
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++)
    {
        unsigned char c = (unsigned char)str[i];
        uint32_t mask = 1u << (c & 0x1F);
        uint32_t index = (c >> 5) & 0x7;

        if ((seen[index] & mask) != 0)
        {
            return false;
        }
        seen[index] |= mask;
    }

    return true;
}

int main(void)
{
    const char *test1 = "abcdef";
    const char *test2 = "hello";
    const char *test3 = "";
    const char *test4 = "aA";
    const char *test5 = NULL;

    printf("Test 1 (%s): %s\n", test1, are_all_characters_unique(test1) ? "unique" : "not unique");
    printf("Test 2 (%s): %s\n", test2, are_all_characters_unique(test2) ? "unique" : "not unique");
    printf("Test 3 (empty): %s\n", are_all_characters_unique(test3) ? "unique" : "not unique");
    printf("Test 4 (%s): %s\n", test4, are_all_characters_unique(test4) ? "unique" : "not unique");
    printf("Test 5 (NULL): %s\n", are_all_characters_unique(test5) ? "unique" : "not unique");

    return 0;
}