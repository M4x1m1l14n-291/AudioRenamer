#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "linkedList.h"

void appendNode(struct ListNode *list, char *name)
{
    if (strlen(list->name) == 0)
    {
        strcpy(list->name, name);
        return;
    }

    struct ListNode *node = malloc(sizeof(struct ListNode));
    strcpy(node->name, name);

    for (; list->next != NULL; list = list->next)
        ;

    list->next = node;
    node->prev = list;
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