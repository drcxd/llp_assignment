#include <malloc.h>
#include <stdio.h>

#include "hof.h"

void foreach(struct node* list, routine_func_type* func)
{
    while (list)
    {
        func(list->v);
        list = list->next;
    }
}

struct node* map(struct node* list, map_func_type* func)
{
    if (!list)
    {
        return NULL;
    }
    struct node* new_head = list_create(func(list->v));
    list = list->next;
    while (list)
    {
        list_add_back(&new_head, func(list->v));
        list = list->next;
    }
    return new_head;
}

void map_mut(struct node* list, map_func_type* func)
{
    while (list)
    {
        list->v = func(list->v);
        list = list->next;
    }
}

int foldl(int accu, accu_func_type* func, struct node* list)
{    
    while (list)
    {
        accu = func(list->v, accu);
        list = list->next;
    }
    return accu;
}

struct node* iterate(int s, unsigned len, map_func_type* func)
{
    if (!len)
    {
        return NULL;
    }
    struct node* head = list_create(s);
    --len;
    while (len)
    {
        s = func(s);
        list_add_back(&head, s);
        --len;
    }
    return head;
}

bool save(struct node* list, const char* fname)
{
    FILE* file = fopen(fname, "w");
    if (!file)
    {
        return false;
    }
    while (list)
    {
        if (fprintf(file, "%d\n", list->v) < 0)
        {
            fclose(file);
            return false;
        }
        list = list->next;
    }
    fclose(file);
    return true;
}

bool load(struct node** plist, const char* fname)
{
    FILE* file = fopen(fname, "r");
    if (!file)
    {
        return false;
    }
    
    struct node* list = *plist;
    while (list)
    {
        if (fscanf(file, "%d", &(list->v)) == EOF)
        {
            return false;
        }
        list = list->next;
    }
    fclose(file);
    return true;
}

bool serialize(struct node* list, const char* fname)
{
    FILE* file = fopen(fname, "wb");
    if (!file)
    {
        return false;
    }
    while (list)
    {
        if (fwrite(&(list->v), 4, 1, file) < 1)
        {
            fclose(file);
            return false;
        }        
        list = list->next;
    }
    fclose(file);
    return true;
}

bool deserialize(struct node** plist, const char* fname)
{
    FILE* file = fopen(fname, "wb");
    if (!file)
    {
        return false;
    }
    struct node* list = *plist;
    while (list)
    {
        if (fread(&(list->v), 4, 1, file) < 1)
        {
            fclose(file);
            return false;
        }
        list = list->next;
    }
    fclose(file);
    return true;
}
