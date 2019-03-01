#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

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
#endif
