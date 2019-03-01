#ifndef _HOF_C_
#define _HOF_C_
#include <stdbool.h>

#include "linked_list.h"

typedef void routine_func_type(int);
typedef int map_func_type(int);
typedef int accu_func_type(int, int);

void foreach(struct node* list, routine_func_type* func);
struct node* map(struct node* list, map_func_type* func);
void map_mut(struct node* list, map_func_type* func);
int foldl(int accu, accu_func_type* func, struct node* list);
struct node* iterate(int s, unsigned len, map_func_type* func);
bool save(struct node* list, const char* fname);
bool load(struct node** plist, const char* fname);
bool serialize(struct node* list, const char* fname);
bool deserialize(struct node** plist, const char* fname);
#endif
