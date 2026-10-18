#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    int length;
} List;

typedef struct ListOfLists {
    List *list;
    struct ListOfLists *next;
} ListOfLists;

ListOfLists *find_min_length_lists(ListOfLists *lol) {
    if (lol == NULL) {
        return NULL;
    }

    int min_len = lol->list->length;
    ListOfLists *current = lol->next;

    while (current != NULL) {
        if (current->list->length < min_len) {
            min_len = current->list->length;
        }
        current = current->next;
    }

    ListOfLists *result_head = NULL;
    ListOfLists *result_tail = NULL;

    current = lol;
    while (current != NULL) {
        if (current->list->length == min_len) {
            ListOfLists *new_node = (ListOfLists *)malloc(sizeof(ListOfLists));
            if (new_node == NULL) {
                ListOfLists *temp = result_head;
                while (temp != NULL) {
                    ListOfLists *next_temp = temp->next;
                    free(temp);
                    temp = next_temp;
                }
                return NULL;
            }
            new_node->list = current->list;
            new_node->next = NULL;

            if (result_head == NULL) {
                result_head = new_node;
                result_tail = new_node;
            } else {
                result_tail->next = new_node;
                result_tail = new_node;
            }
        }
        current = current->next;
    }

    return result_head;
}

List *create_list(int length) {
    List *l = (List *)malloc(sizeof(List));
    if (l == NULL) return NULL;
    l->head = NULL;
    l->length = length;
    return l;
}

ListOfLists *create_list_of_lists(List *l) {
    ListOfLists *lol = (ListOfLists *)malloc(sizeof(ListOfLists));
    if (lol == NULL) return NULL;
    lol->list = l;
    lol->next = NULL;
    return lol;
}

void free_list_of_lists(ListOfLists *lol) {
    ListOfLists *current = lol;
    while (current != NULL) {
        ListOfLists *next = current->next;
        free(current);
        current = next;
    }
}

void free_list(List *l) {
    if (l != NULL) {
        free(l);
    }
}

int main() {
    List *l1 = create_list(5);
    List *l2 = create_list(2);
    List *l3 = create_list(8);
    List *l4 = create_list(2);

    if (l1 == NULL || l2 == NULL || l3 == NULL || l4 == NULL) {
        free_list(l1);
        free_list(l2);
        free_list(l3);
        free_list(l4);
        return 1;
    }

    ListOfLists *lol1 = create_list_of_lists(l1);
    ListOfLists *lol2 = create_list_of_lists(l2);
    ListOfLists *lol3 = create_list_of_lists(l3);
    ListOfLists *lol4 = create_list_of_lists(l4);

    if (lol1 == NULL || lol2 == NULL || lol3 == NULL || lol4 == NULL) {
        free_list_of_lists(lol1);
        free_list_of_lists(lol2);
        free_list_of_lists(lol3);
        free_list_of_lists(lol4);
        free_list(l1);
        free_list(l2);
        free_list(l3);
        free_list(l4);
        return 1;
    }

    lol1->next = lol2;
    lol2->next = lol3;
    lol3->next = lol4;

    ListOfLists *min_lists = find_min_length_lists(lol1);

    if (min_lists != NULL) {
        ListOfLists *curr = min_lists;
        while (curr != NULL) {
            printf("List length: %d\n", curr->list->length);
            curr = curr->next;
        }
    }

    free_list_of_lists(min_lists);
    free_list_of_lists(lol1);
    free_list(l1);
    free_list(l2);
    free_list(l3);
    free_list(l4);

    return 0;
}