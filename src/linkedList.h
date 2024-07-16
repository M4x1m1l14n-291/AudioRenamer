typedef struct list_node
{
    char name[256];
    struct list_node *next;
    struct list_node *prev;
} list_node;

void appendNode(struct list_node *list, char *name);
void swapNodes(list_node *node1, list_node *node2);

void alphaSort(list_node *list);

void freeNodes(list_node *node);

void printList(struct list_node *list);