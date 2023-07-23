#pragma once

#include <stddef.h>
#include <stdio.h>

#define MID(x)      ( ((x) - 1) / 2 )
#define END1(x)     ( 2 * (x) + 1 )
#define GT(v1, v2)  ( ((v1).dist - (v2).dist) > 0 )


#define HEAP_START_CAP 0x10


typedef struct heap_entry *HEntry;
typedef struct binary_heap *BHeap;

struct binary_heap {
    HEntry data;
    size_t sz;
    size_t capacity;
};

struct heap_entry {
    ssize_t dist;
    size_t vertex;
};


/*  Standard heap functions.  */

HEntry heap_get_top (const BHeap heap);

int heap_extract_top (BHeap heap, HEntry top);

void heap_insert (BHeap heap, const HEntry entry);

void heap_free (BHeap heap);


/*  Little functions.  */

/**
 * @brief Fix binheap after insertion.
 * 
 * @param heap [IN] - heap.
 */
void fix_end (BHeap heap);

/**
 * @brief Fix binheap after deletion.
 * 
 * @param heap [IN] - heap.
 */
void fix_mid (BHeap heap);

/**
 * @brief Take least end of two corresponding to middle.
 * (Middle must be less then both ends.)
 * 
 * @param heap [IN] - heap.
 */
size_t least_end (BHeap heap, const size_t mid);

/**
 * @brief Swap two entries in heap.
 * 
 * @param heap [IN] - heap
 * @param lhs  [IN] - first entry to swap
 * @param rhs  [IN] - second entry to swap
 */
void heap_swap (BHeap heap, size_t lhs, size_t rhs);

