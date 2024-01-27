#ifndef UINT32_MAXHEAP_H
#define UINT32_MAXHEAP_H

#include "num_arrays/uint32_array.h"

#define HEAP_NAME uint32_maxheap
#define ARRAY_TYPE uint32_t
#define ARRAY_NAME uint32_array
#define MAXHEAP
#include "heap.h"
#undef HEAP_NAME
#undef ARRAY_TYPE
#undef ARRAY_NAME
#undef MAXHEAP

#endif