#ifndef UINT32_MINHEAP_H
#define UINT32_MINHEAP_H

#include "num_array/uint32_array.h"

#define HEAP_NAME uint32_minheap
#define ARRAY_TYPE uint32_t
#define ARRAY_NAME uint32_array
#include "heap.h"
#undef HEAP_NAME
#undef ARRAY_TYPE
#undef ARRAY_NAME

#endif