#ifndef FLOAT_MAXHEAP_H
#define FLOAT_MAXHEAP_H

#include "num_arrays/float_array.h"

#define HEAP_NAME float_maxheap
#define ARRAY_TYPE float
#define ARRAY_NAME float_array
#define MAXHEAP
#include "heap.h"
#undef HEAP_NAME
#undef ARRAY_TYPE
#undef ARRAY_NAME
#undef MAXHEAP

#endif