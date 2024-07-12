typedef struct ListNode
{
    char name[256];
    struct ListNode *next;
    struct ListNode *prev;
} ListNode;

void appendNode(struct ListNode *list, char *name);
void swapNodes(ListNode *node1, ListNode *node2);

void sortListAlpha(ListNode *list);

void printList(struct ListNode *list);