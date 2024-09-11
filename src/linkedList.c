#include "linkedList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void appendNode(struct ListNode *list, char *name) {
    if (strlen(list->name) == 0) {
        strcpy(list->name, name);
        return;
    }

    struct ListNode *node = malloc(sizeof(struct ListNode));
    strcpy(node->name, name);

    for (; list->next != NULL; list = list->next);

    list->next = node;
    node->prev = list;
}

void swapNodes(ListNode *node1, ListNode *node2) {
    char temp[256];
    strcpy(temp, node1->name);
    strcpy(node1->name, node2->name);
    strcpy(node2->name, temp);
}

void editnode(ListNode *node, char *name) { strcpy(node->name, name); }

void alphaSort(ListNode *list) {
    ListNode *first = list;

    int sorted = 0;

    while (!sorted) {
        list = first;
        sorted = 1;

        while (list->next != NULL) {
            if (strcmp(list->name, list->next->name) > 0) {
                swapNodes(list, list->next);
                sorted = 0;
            }

            list = list->next;
        }
    }
}

void freeNodes(ListNode *node) {
    if (node->next != NULL) freeNodes(node->next);
    free(node);
}

void printList(struct ListNode *list) {
    for (; list; list = list->next) printf("%s\n", list->name);
}