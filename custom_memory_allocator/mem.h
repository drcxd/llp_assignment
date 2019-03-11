#ifndef _MEM_H_
#define _MEM_H_

#include <stdbool.h>
#include <stddef.h>

#pragma pack(push, 1)
struct mem {
    struct mem *next;
    size_t capacity;
    bool is_free;
};
#pragma pack(pop)

void *_malloc(size_t query);
void _free(void *mem);
void *heap_init(size_t initial_size);

#endif /* _MEM_H_ */
