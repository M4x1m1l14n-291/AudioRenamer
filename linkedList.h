typedef struct ListNode
{
    char name[256];
    struct ListNode *next;
} ListNode;

void appendNode(struct ListNode *list, char *name);
void removeNode(ListNode *node);

void printList(struct ListNode *list);