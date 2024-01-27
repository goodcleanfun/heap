#ifndef DOUBLE_MAXHEAP_H
#define DOUBLE_MAXHEAP_H

#include "num_arrays/float_array.h"

#define HEAP_NAME double_maxheap
#define ARRAY_TYPE double
#define ARRAY_NAME double_array
#define MAXHEAP
#include "heap.h"
#undef HEAP_NAME
#undef ARRAY_TYPE
#undef ARRAY_NAME
#undef MAXHEAP

#endif