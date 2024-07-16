#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "linkedList.h"

void appendNode(struct list_node *list, char *name)
{
    if (strlen(list->name) == 0)
    {
        strcpy(list->name, name);
        return;
    }

    struct list_node *node = malloc(sizeof(struct list_node));
    strcpy(node->name, name);

    for (; list->next != NULL; list = list->next)
        ;

    list->next = node;
    node->prev = list;
}

void swapNodes(list_node *node1, list_node *node2)
{
    char temp[256];
    strcpy(temp, node1->name);
    strcpy(node1->name, node2->name);
    strcpy(node2->name, temp);
}

void alphaSort(list_node *list)
{
    list_node *first = list;

    int sorted = 0;

    while (!sorted)
    {
        list = first;
        sorted = 1;

        while (list->next != NULL)
        {
            if (strcmp(list->name, list->next->name) > 0)
            {
                swapNodes(list, list->next);
                sorted = 0;
            }

            list = list->next;
        }
    }
}

void freeNodes(list_node *node)
{
    if (node->next != NULL)
        freeNodes(node->next);
    free(node);
}

void printList(struct list_node *list)
{
    for (; list; list = list->next)
        printf("%s\n", list->name);
}