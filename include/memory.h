#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stddef.h>

extern void my_alloc_init(void);
extern void *my_malloc(size_t size);
extern void my_free(void *ptr);

#endif
