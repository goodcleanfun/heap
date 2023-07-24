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
static inline void name##_push(array_type *array, type x) {                     \
    array_type##_push(array, x);                                                \
    size_t i = array->n - 1;                                                    \
    type *a = array->a;                                                         \
    while (i > 0) {                                                             \
        size_t p = heap_parent(i);                                              \
        if (precedes(a[p], a[i])) {                                             \
            type tmp = a[i];                                                    \
            a[i] = a[p];                                                        \
            a[p] = tmp;                                                         \
            i = p;                                                              \
        } else {                                                                \
            break;                                                              \
        }                                                                       \
    }                                                                           \
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
    size_t i = 0;                                                               \
                                                                                \
    while (true) {                                                              \
        size_t left = heap_left(i);                                             \
        size_t right = heap_right(i);                                           \
        size_t priority = i;                                                    \
        if (left < array->n && precedes(a[left], a[priority])) {                \
            priority = left;                                                    \
        } else if (right < array->n && precedes(a[right], a[priority])) {       \
            priority = right;                                                   \
        }                                                                       \
                                                                                \
        if (priority != i) {                                                    \
            type tmp = a[i];                                                    \
            a[i] = a[priority];                                                 \
            a[priority] = tmp;                                                  \
            i = priority;                                                       \
        } else {                                                                \
            break;                                                              \
        }                                                                       \
    }                                                                           \
                                                                                \
    *result = root;                                                             \
    return true;                                                                \
}                                                                               \
                                                                                \
static inline bool name##_siftdown(array_type *array, size_t start, size_t end) { \
    if (start > array->n || end > array->n) return false;                       \
    size_t root = start;                                                        \
    type *a = array->a;                                                         \
    while (heap_left(root) <= end) {                                            \
        size_t child = heap_left(root);                                         \
        size_t swap = root;                                                     \
        if (precedes(a[child], a[swap])) {                                      \
            swap = child;                                                       \
        }                                                                       \
        if (child + 1 <= end && precedes(a[child + 1], a[swap])) {              \
            swap = child + 1;                                                   \
        }                                                                       \
        if (swap == root) {                                                     \
            break;                                                              \
        } else {                                                                \
            type tmp = a[root];                                                 \
            a[root] = a[swap];                                                  \
            a[swap] = tmp;                                                      \
            root = swap;                                                        \
        }                                                                       \
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