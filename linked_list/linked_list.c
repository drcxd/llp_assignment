#include <stdio.h>
#include <malloc.h>

struct node
{
    int v;
    struct node* next;
};

struct node* list_create(int v);
void list_add_front(struct node** plist, int v);
void list_add_back(struct node** plist, int v);
int list_get(struct node* list, int i);
void list_free(struct node* list);
int list_length(struct node* list);
struct node* list_node_at(struct node* list, int i);
int list_sum(struct node* list);

int main(void)
{
    int v;
    struct node* head;
    if (scanf("%d", &v) == EOF)
    {
        return 0; /* the list is empty */
    }

    head = list_create(v);
    
    while (scanf("%d", &v) != EOF)
    {
        list_add_front(&head, v);
    }
    printf("the sum of the list is %d\n", list_sum(head));
    return 0;
}

struct node* list_create(int v)
{
    struct node* list = (struct node*)malloc(sizeof(struct node));
    list->v = v;
    list->next = NULL;
    return list;
}

void list_add_front(struct node** plist, int v)
{
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->v = v;
    new_node->next = *plist;
    *plist = new_node;
}

void list_add_back(struct node** plist, int v)
{
    struct node* head = *plist;
    struct node* new_node = (struct node*)malloc(sizeof(struct node));    
    
    new_node->v = v;
    new_node->next = NULL;
    
    while (head)
    {
        head = head->next;
    }
    head->next = new_node;
}

int list_get(struct node* list, int i)
{
    while (i > 0 && list)
    {
        list = list->next;
    }
    if (!list)
    {
        return 0; /* out of bound */
    }
    return list->v;
}

void list_free(struct node* list)
{
    struct node* temp;
    while (list)
    {
        temp = list->next;
        free(list);
        list = temp;
    }
}

int list_length(struct node* list)
{
    int len = 0;
    while (list)
    {
        list = list->next;
        ++len;
    }
    return len;
}

struct node* list_node_at(struct node* list, int i)
{
    while (i > 0 && list)
    {
        list = list->next;
    }
    return list;
}

int list_sum(struct node* list)
{
    int sum = 0;
    while (list)
    {
        sum += list->v;
        list = list->next;
    }
    return sum;
}
