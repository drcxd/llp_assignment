#include "mem.h"

#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>

static const int BLOCK_MIN_SIZE = 4096;

static struct mem *head = NULL;

void *heap_init(size_t initial_size)
{
    if (initial_size < BLOCK_MIN_SIZE)
    {
        initial_size = BLOCK_MIN_SIZE;
    }
    head = mmap(NULL, initial_size + sizeof(struct mem), PROT_READ | PROT_READ, MAP_ANONYMOUS, -1, 0);
    if (!head)
    {
        fprintf(stderr, "fatal error, can not initialize the allocator\n");
        return NULL;
    }
    head->next = NULL;
    head->capacity = initial_size + sizeof(struct mem);
    head->is_free = true;
    return head;
}

void *_malloc(size_t query)
{
    if (query < BLOCK_MIN_SIZE)
    {
        query = BLOCK_MIN_SIZE;
    }
    struct mem *cur = head;
    struct mem *prev = NULL;
    while (cur)
    {
        if (!cur->is_free || cur->capacity - sizeof(struct mem) < query)
        {
            prev = cur;
            cur = cur->next;
        }        
        else
        {
            cur->is_free = false;
            return (void*)(cur + 1);
        }
    }
    cur = mmap(NULL, query + sizeof(struct mem), PROT_READ | PROT_READ, MAP_ANONYMOUS, -1, 0);
    cur->next = NULL;
    cur->capacity = query + sizeof(struct mem);
    prev->next = cur;
    return (void*)(cur + 1);
}

void _free(void *mem)
{
    struct mem *header = ((struct mem *)mem) - 1;
    header->is_free = true;    
}
