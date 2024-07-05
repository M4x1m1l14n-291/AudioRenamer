#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "linkedList.h"

void appendNode(struct ListNode *list, char *name)
{
    struct ListNode *node = malloc(sizeof(struct ListNode));

    strcpy(node->name, name);
    node->next = NULL;

    for (; list->next != NULL; list = list->next)
        ;

    list->next = node;
}

void removeNode(ListNode *node)
{
}

void printList(struct ListNode *list)
{
    if (list == NULL)
        return;

    for (; list; list = list->next)
        printf("%s\n", list->name);
}