#ifndef UINT64_MAXHEAP_H
#define UINT64_MAXHEAP_H

#include "num_array/uint64_array.h"

#define HEAP_NAME uint64_maxheap
#define ARRAY_TYPE uint64_t
#define ARRAY_NAME uint64_array
#define MAXHEAP
#include "heap.h"
#undef HEAP_NAME
#undef ARRAY_TYPE
#undef ARRAY_NAME
#undef MAXHEAP

#endif