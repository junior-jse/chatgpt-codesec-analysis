#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint64_t key;
    uint64_t value;
    unsigned char used;
} Entry;

typedef struct {
    Entry *entries;
    size_t capacity;
    size_t size;
} HashMap;

static uint64_t hash_uint64(uint64_t value)
{
    value ^= value >> 30;
    value *= UINT64_C(0xbf58476d1ce4e5b9);
    value ^= value >> 27;
    value *= UINT64_C(0x94d049bb133111eb);
    value ^= value >> 31;
    return value;
}

static int map_init(HashMap *map, size_t capacity)
{
    map->entries = calloc(capacity, sizeof(*map->entries));
    if (map->entries == NULL) {
        return 0;
    }

    map->capacity = capacity;
    map->size = 0;
    return 1;
}

static void map_destroy(HashMap *map)
{
    free(map->entries);
    map->entries = NULL;
    map->capacity = 0;
    map->size = 0;
}

static int map_find(const HashMap *map, uint64_t key, uint64_t *value)
{
    size_t index = (size_t)(hash_uint64(key) % map->capacity);

    while (map->entries[index].used != 0U) {
        if (map->entries[index].key == key) {
            *value = map->entries[index].value;
            return 1;
        }
        index = (index + 1U) % map->capacity;
    }

    return 0;
}

static int map_insert_raw(HashMap *map, uint64_t key, uint64_t value)
{
    size_t index = (size_t)(hash_uint64(key) % map->capacity);

    while (map->entries[index].used != 0U) {
        if (map->entries[index].key == key) {
            map->entries[index].value = value;
            return 1;
        }
        index = (index + 1U) % map->capacity;
    }

    map->entries[index].used = 1U;
    map->entries[index].key = key;
    map->entries[index].value = value;
    ++map->size;
    return 1;
}

static int map_resize(HashMap *map)
{
    HashMap replacement;
    size_t i;
    size_t new_capacity;

    if (map->capacity > SIZE_MAX / 2U) {
        return 0;
    }

    new_capacity = map->capacity * 2U;
    if (!map_init(&replacement, new_capacity)) {
        return 0;
    }

    for (i = 0; i < map->capacity; ++i) {
        if (map->entries[i].used != 0U) {
            map_insert_raw(&replacement, map->entries[i].key,
                           map->entries[i].value);
        }
    }

    free(map->entries);
    *map = replacement;
    return 1;
}

static int map_insert(HashMap *map, uint64_t key, uint64_t value)
{
    if (map->size >= map->capacity - map->capacity / 4U) {
        if (!map_resize(map)) {
            return 0;
        }
    }

    return map_insert_raw(map, key, value);
}

static int add_checked(uint64_t left, uint64_t right, uint64_t *result)
{
    if (UINT64_MAX - left < right) {
        return 0;
    }

    *result = left + right;
    return 1;
}

static int max_sum(uint64_t n, HashMap *memo, uint64_t *result)
{
    uint64_t a;
    uint64_t b;
    uint64_t c;
    uint64_t d;
    uint64_t sum;

    if (n == 0U) {
        *result = 0U;
        return 1;
    }

    if (map_find(memo, n, result)) {
        return 1;
    }

    if (!max_sum(n / 2U, memo, &a) ||
        !max_sum(n / 3U, memo, &b) ||
        !max_sum(n / 4U, memo, &c) ||
        !max_sum(n / 5U, memo, &d)) {
        return 0;
    }

    if (!add_checked(a, b, &sum) ||
        !add_checked(sum, c, &sum) ||
        !add_checked(sum, d, &sum)) {
        return 0;
    }

    *result = sum > n ? sum : n;
    return map_insert(memo, n, *result);
}

int main(void)
{
    HashMap memo;
    uint64_t n;
    uint64_t result;
    int scan_result;

    errno = 0;
    /* Possible weaknesses found:
     * Flawfinder scanf: The scanf() family's %s operation, without a limit specification, permits buffer overflows (CWE-120, CWE-20). Specify a limit to %s, or use a different input function. If the scanf format is influenceable by an attacker, it's exploitable. (risk 4, buffer)
     */
    scan_result = scanf("%" SCNu64, &n);
    if (scan_result != 1 || errno == ERANGE) {
        fputs("Invalid input\n", stderr);
        return EXIT_FAILURE;
    }

    if (!map_init(&memo, 128U)) {
        fputs("Memory allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    if (!max_sum(n, &memo, &result)) {
        fputs("Computation failed\n", stderr);
        map_destroy(&memo);
        return EXIT_FAILURE;
    }

    if (printf("%" PRIu64 "\n", result) < 0) {
        fputs("Output failed\n", stderr);
        map_destroy(&memo);
        return EXIT_FAILURE;
    }

    map_destroy(&memo);
    return EXIT_SUCCESS;
}