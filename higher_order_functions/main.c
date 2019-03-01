#include <stdio.h>
#include <limits.h>

#include "linked_list.h"
#include "hof.h"

static void print_with_space(int i)
{
    printf("%d ", i);
}

static void print_with_newline(int i)
{
    printf("%d\n", i);
}

static int map_square(int i)
{
    return i * i;
}

static int map_cube(int i)
{
    return i * i * i;
}

static int map_mod3(int i)
{
    return i % 3;
}

static int accu_sum(int x, int accu)
{
    return x + accu;
}

static int accu_min(int x, int accu)
{
    return x < accu ? x : accu;
}

static int accu_max(int x, int accu)
{
    return x > accu ? x : accu;
}

static int it_pow2(int x)
{
    return 2 * x;
}

int main(void)
{
    int v;
    struct node* head;
    if (scanf("%d", &v) == EOF)
    {
        return 0;
    }

    head = list_create(v);

    while (scanf("%d", &v) != EOF)
    {
        list_add_back(&head, v);
    }

    foreach(head, print_with_space);
    puts("");
    foreach(head, print_with_newline);

    struct node* new_list1 = map(head, map_square);
    struct node* new_list2 = map(head, map_cube);
    foreach(new_list1, print_with_newline);
    foreach(new_list2, print_with_newline);

    printf("sum of the list is %d\n", foldl(0, accu_sum, head));
    printf("min of the list is %d\n", foldl(INT_MAX, accu_min, head));
    printf("max of the list is %d\n", foldl(INT_MIN, accu_max, head));

    map_mut(head, map_mod3);
    foreach(head, print_with_newline);

    struct node* new_list3 = iterate(1, 10, it_pow2);
    foreach(new_list3, print_with_newline);
    
    save(new_list3, "list.txt");
    load(&new_list3, "list.txt");
    puts("After load: ");
    foreach(new_list3, print_with_newline);

    save(new_list3, "list.bin");
    load(&new_list3, "list.bin");
    puts("After binary load: ");
    foreach(new_list3, print_with_newline);

    free(head);
    free(new_list1);
    free(new_list2);
    free(new_list3);
    
    return 0;
}
