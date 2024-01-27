#ifndef UINT64_MINHEAP_H
#define UINT64_MINHEAP_H

#include "num_arrays/uint64_array.h"

#define HEAP_NAME uint64_minheap
#define ARRAY_TYPE uint64_t
#define ARRAY_NAME uint64_array
#include "heap.h"
#undef HEAP_NAME
#undef ARRAY_TYPE
#undef ARRAY_NAME

#endif