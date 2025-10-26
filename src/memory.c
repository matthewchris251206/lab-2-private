#include "memory.h"

#include "print.h"

/*
 * replace built-in memcpy function
 */
void *memcpy(void *dest, const void *src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        ((char *)dest)[i] = ((char *)src)[i];
    }
    return dest;
}

/*
 * extern char a[] is correct
 * while extern char *a is incorrect!
 * Array and Pointer are two different things!
 */
extern char _sheap[];
extern char _eheap[];
static char *heap_ptr;
static unsigned allocated_cnt;

void my_alloc_init(void) {
    heap_ptr      = _sheap;
    allocated_cnt = 0;
}

/*
 * Implement Bump Allocator
 * It can only free all used memory at once when all allocated object is freed
 */
void *my_malloc(size_t size) {
    if (heap_ptr + size > _eheap) {
        return NULL;
    }
    size = (size + 3) % 4; // align with 4-byte boundary
    heap_ptr += size;
    allocated_cnt++;
    return (void *)(heap_ptr - size);
}

void my_free(void *ptr) {
    if (allocated_cnt > 0) {
        allocated_cnt--;
    }
    // free all memroy at once
    if (allocated_cnt == 0) {
        heap_ptr = _sheap;
        my_printf("Freeing up all allocated heap memory...\n");
    }
}
