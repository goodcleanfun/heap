#include <stdbool.h>

#ifndef HEAP_H
#define HEAP_H

#define maxheap_cmp(a, b) ((a) > (b))
#define minheap_cmp(a, b) ((a) < (b))

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

#define HEAP_INIT(name, type, array_type, precedes)                             \
static inline bool name##_siftdown(array_type *array, size_t start_pos, size_t pos) { \
    if (start_pos > array->n || pos > array->n) return false;                       \
    type *a = array->a;                                                         \
    type new_item = a[pos];                                                     \
    while (pos > start_pos) {                                                   \
        size_t parent_pos = heap_parent(pos);                                   \
        type parent = a[parent_pos];                                            \
        if (!precedes(new_item, parent)) {                                      \
            break;                                                              \
        }                                                                       \
        a[pos] = parent;                                                        \
        a[parent_pos] = new_item;                                               \
        pos = parent_pos;                                                       \
    }                                                                           \
    return true;                                                                \
}                                                                               \
                                                                                \
static inline bool name##_siftup(array_type *array, size_t i) {                 \
    size_t start = i;                                                           \
    size_t end = array->n;                                                      \
    if (i >= end) {                                                             \
        return false;                                                           \
    }                                                                           \
    type *a = array->a;                                                         \
    size_t limit = end >> 1;                                                    \
    while (i < limit) {                                                         \
        size_t child_i = heap_left(i);                                          \
        if (child_i + 1 < end && precedes(a[child_i + 1], a[child_i])) {        \
            child_i++;                                                          \
        }                                                                       \
        if (precedes(a[i], a[child_i])) {                                       \
            break;                                                              \
        }                                                                       \
        type tmp = a[child_i];                                                  \
        a[child_i] = a[i];                                                      \
        a[i] = tmp;                                                             \
        i = child_i;                                                            \
    }                                                                           \
    name##_siftdown(array, start, i);                                           \
    return true;                                                                \
}                                                                               \
                                                                                \
static inline bool name##_push(array_type *array, type x) {                     \
    array_type##_push(array, x);                                                \
    return name##_siftdown(array, 0, array->n - 1);                             \
}                                                                               \
                                                                                \
static inline bool name##_pop(array_type *array, type *result) {                \
    if (array->n == 0) {                                                        \
        return false;                                                           \
    }                                                                           \
    type *a = array->a;                                                         \
    type root = a[0];                                                           \
    type last;                                                                  \
    if (!array_type##_pop(array, &last)) {                                      \
        return false;                                                           \
    }                                                                           \
    if (array->n == 0) {                                                        \
        *result = root;                                                         \
        return true;                                                            \
    }                                                                           \
    a[0] = last;                                                                \
    if (!name##_siftup(array, 0)) {                                             \
        return false;                                                           \
    }                                                                           \
    *result = root;                                                             \
    return true;                                                                \
}                                                                               \
                                                                                \
static inline bool name##_cache_friendly_heapify(array_type *array) {           \
    size_t m = array->n >> 1;                                                   \
    size_t leftmost = keep_top_bit(m + 1) - 1;                                  \
    size_t mp = m >> 1;                                                         \
    size_t i, j;                                                                \
    for (i = leftmost - 1; i >= mp; i--) {                                      \
        j = i;                                                                  \
        while (true) {                                                          \
            if (!name##_siftup(array, j)) return false;                         \
            if (!(j & 1)) {                                                     \
                break;                                                          \
            }                                                                   \
            j >>= 1;                                                            \
        }                                                                       \
    }                                                                           \
    for (i = m - 1; i >= leftmost; i--) {                                       \
        j = i;                                                                  \
        while (true) {                                                          \
            if (!name##_siftup(array, j)) return false;                         \
            if (!(j & 1)) {                                                     \
                break;                                                          \
            }                                                                   \
            j >>= 1;                                                            \
        }                                                                       \
    }                                                                           \
    return true;                                                                \
}                                                                               \
                                                                                \
static inline bool name##_heapify(array_type *array) {                          \
    size_t n = array->n;                                                        \
    if (n > CACHE_FRIENDLY_THRESHOLD) {                                         \
        return name##_cache_friendly_heapify(array);                            \
    } else {                                                                    \
        size_t start = heap_parent(n - 1);                                      \
        bool success;                                                           \
        while (start > 0) {                                                     \
            success = name##_siftdown(array, start, n - 1);                     \
            if (!success) return false;                                         \
            start--;                                                            \
        }                                                                       \
        success = name##_siftdown(array, start, n - 1);                         \
        if (!success) return false;                                             \
    }                                                                           \
    return true;                                                                \
}

#define MINHEAP_INIT(name, type, array_type) HEAP_INIT(name, type, array_type, minheap_cmp)
#define MAXHEAP_INIT(name, type, array_type) HEAP_INIT(name, type, array_type, maxheap_cmp)


#endif