#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VALUE_LEN 256

typedef struct {
    int id;
    char value[MAX_VALUE_LEN];
} Tuple;

typedef struct {
    Tuple *items;
    size_t count;
} TupleList;

typedef struct {
    const int *order;
    size_t count;
} OrderList;

static size_t safe_strnlen(const char *str, size_t maxlen) {
    size_t i;
    for (i = 0; i < maxlen; i++) {
        if (str[i] == '\0') {
            return i;
        }
    }
    return maxlen;
}

static const Tuple *find_tuple_by_id(const TupleList *tuples, int id) {
    size_t i;
    for (i = 0; i < tuples->count; i++) {
        if (tuples->items[i].id == id) {
            return &tuples->items[i];
        }
    }
    return NULL;
}

int rearrange_tuples(const TupleList *input, const OrderList *order, TupleList *output) {
    size_t i;

    if (input == NULL || order == NULL || output == NULL) {
        return -1;
    }

    if (input->count == 0 || order->count == 0) {
        output->items = NULL;
        output->count = 0;
        return 0;
    }

    output->items = malloc(order->count * sizeof(Tuple));
    if (output->items == NULL) {
        return -1;
    }

    output->count = order->count;

    for (i = 0; i < order->count; i++) {
        const Tuple *found = find_tuple_by_id(input, order->order[i]);
        if (found == NULL) {
            free(output->items);
            output->items = NULL;
            output->count = 0;
            return -1;
        }

        if (safe_strnlen(found->value, MAX_VALUE_LEN) >= MAX_VALUE_LEN) {
            free(output->items);
            output->items = NULL;
            output->count = 0;
            return -1;
        }

        output->items[i].id = found->id;
        strncpy(output->items[i].value, found->value, MAX_VALUE_LEN - 1);
        output->items[i].value[MAX_VALUE_LEN - 1] = '\0';
    }

    return 0;
}

int main(void) {
    Tuple input_items[] = {
        {1, "one"},
        {2, "two"},
        {3, "three"},
        {4, "four"}
    };
    const int order_items[] = {3, 1, 4, 2};

    TupleList input;
    OrderList order;
    TupleList output;
    size_t i;

    input.items = input_items;
    input.count = sizeof(input_items) / sizeof(input_items[0]);

    order.order = order_items;
    order.count = sizeof(order_items) / sizeof(order_items[0]);

    if (rearrange_tuples(&input, &order, &output) != 0) {
        fprintf(stderr, "Error rearranging tuples\n");
        return 1;
    }

    for (i = 0; i < output.count; i++) {
        printf("%d: %s\n", output.items[i].id, output.items[i].value);
    }

    free(output.items);
    output.items = NULL;
    output.count = 0;

    return 0;
}