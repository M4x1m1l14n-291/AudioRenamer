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

void switchNodes(ListNode *node1, ListNode *node2)
{
    char temp[] = node1->name;
    strcpy(node1->name, node2->name);
    strcpy(node2->name, temp);
}

void printList(struct ListNode *list)
{
    if (list == NULL)
        return;

    for (; list; list = list->next)
        printf("%s\n", list->name);
}