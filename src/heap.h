#include <stdbool.h>

#ifndef HEAP_H
#define HEAP_H

#define CACHE_FRIENDLY_THRESHOLD 2500

static inline size_t heap_parent(size_t i) {
    return (i - 1) >> 1;
}

static inline size_t heap_left(size_t i) {
    return (i << 1) + 1;
}

static inline size_t heap_right(size_t i) {
    return (i << 1) + 2;
}

static inline size_t keep_top_bit(size_t n) {
    size_t i = 0;
    while (n > 1) {
        n >>= 1;
        i++;
    }
    return n << i;
}

#endif

#ifndef HEAP_NAME
#error "Must define HEAP_NAME"
#endif

#ifndef ARRAY_NAME
#error "Must define ARRAY_NAME"
#endif

#ifndef ARRAY_TYPE
#error "Must define ARRAY_TYPE"
#endif

#define CONCAT_(a, b) a ## b
#define CONCAT(a, b) CONCAT_(a, b)
#define HEAP_FUNC(name) CONCAT(HEAP_NAME, _##name)
#define ARRAY_FUNC(name) CONCAT(ARRAY_NAME, _##name)

#ifdef MAXHEAP
#define PRECEDES(a, b) ((a) > (b))
#else
#define PRECEDES(a, b) ((a) < (b))
#endif


static inline bool HEAP_FUNC(siftdown)(ARRAY_NAME *array, size_t start_pos, size_t pos) {
    if (start_pos > array->n || pos > array->n) return false;
    ARRAY_TYPE *a = array->a;
    ARRAY_TYPE new_item = a[pos];
    while (pos > start_pos) {
        size_t parent_pos = heap_parent(pos);
        ARRAY_TYPE parent = a[parent_pos];
        if (!PRECEDES(new_item, parent)) {
            break;
        }
        a[pos] = parent;
        a[parent_pos] = new_item;
        pos = parent_pos;
    }
    return true;
}

static inline bool HEAP_FUNC(siftup)(ARRAY_NAME *array, size_t i) {
    size_t start = i;
    size_t end = array->n;
    if (i >= end) {
        return false;
    }
    ARRAY_TYPE *a = array->a;
    size_t limit = end >> 1;
    while (i < limit) {
        size_t child_i = heap_left(i);
        if (child_i + 1 < end && PRECEDES(a[child_i + 1], a[child_i])) {
            child_i++;
        }
        if (PRECEDES(a[i], a[child_i])) {
            break;
        }
        ARRAY_TYPE tmp = a[child_i];
        a[child_i] = a[i];
        a[i] = tmp;
        i = child_i;
    }
    HEAP_FUNC(siftdown)(array, start, i);
    return true;
}

static inline bool HEAP_FUNC(push)(ARRAY_NAME *array, ARRAY_TYPE x) {
    ARRAY_FUNC(push)(array, x);
    return HEAP_FUNC(siftdown)(array, 0, array->n - 1);
}

static inline bool HEAP_FUNC(pop)(ARRAY_NAME *array, ARRAY_TYPE *result) {
    if (array->n == 0) {
        return false;
    }
    ARRAY_TYPE *a = array->a;
    ARRAY_TYPE root = a[0];
    ARRAY_TYPE last;
    if (!ARRAY_FUNC(pop)(array, &last)) {
        return false;
    }
    if (array->n == 0) {
        *result = root;
        return true;
    }
    a[0] = last;
    if (!HEAP_FUNC(siftup)(array, 0)) {
        return false;
    }
    *result = root;
    return true;
}

static inline bool HEAP_FUNC(cache_friendly_heapify)(ARRAY_NAME *array) {
    size_t m = array->n >> 1;
    size_t leftmost = keep_top_bit(m + 1) - 1;
    size_t mp = m >> 1;
    size_t i, j;
    for (i = leftmost - 1; i >= mp; i--) {
        j = i;
        while (true) {
            if (!HEAP_FUNC(siftup)(array, j)) return false;
            if (!(j & 1)) {
                break;
            }
            j >>= 1;
        }
    }
    for (i = m - 1; i >= leftmost; i--) {
        j = i;
        while (true) {
            if (!HEAP_FUNC(siftup)(array, j)) return false;
            if (!(j & 1)) {
                break;
            }
            j >>= 1;
        }
    }
    return true;
}

static inline bool HEAP_FUNC(heapify)(ARRAY_NAME *array) {
    size_t n = array->n;
    if (n > CACHE_FRIENDLY_THRESHOLD) {
        return HEAP_FUNC(cache_friendly_heapify)(array);
    } else {
        size_t start = heap_parent(n - 1);
        bool success;
        while (start > 0) {
            success = HEAP_FUNC(siftdown)(array, start, n - 1);
            if (!success) return false;
            start--;
        }
        success = HEAP_FUNC(siftdown)(array, start, n - 1);
        if (!success) return false;
    }
    return true;
}

#undef CONCAT_
#undef CONCAT
#undef PRECEDES
#undef HEAP_FUNC
#undef ARRAY_FUNC