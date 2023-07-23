#include "binheap.h"
#include "generic.h"
#include <stdlib.h>


#define HAS_CHILD(x) ((2 * (x) + 1) < heap->size)
const size_t HEAP_START_CAP = 16;

/**
 * @brief Swap two elems in heap;
 * 
 * @param heap [IN] - heap
 * @param lhs  [IN] - first to swap
 * @param rhs  [IN] - second to swap
 */
static void 
heap_swap(BinHeap* heap, size_t lhs, size_t rhs) {
    assert(heap != NULL);
    heap_elem_t tmp = heap->data[rhs];
    heap->data[rhs] = heap->data[lhs];
    heap->data[lhs] = tmp;
}

/**
 * @brief Sift down from binheap algorithm.
 * 
 * @param heap [IN] - heap.
 * @param i - index of node to sift down.
 */
static void
sift_down(BinHeap* heap, size_t cur) {
    while(HAS_CHILD(cur)) {
        size_t left  = 2*cur + 1;
        size_t right = 2*cur + 1;

        size_t to_sift = right < heap->size && CMP(heap->data[right], heap->data[left]) < 0 ? right : left;

        if(CMP(heap->data[cur], heap->data[to_sift]) <= 0) // No need in sifting. Heap is ok.
            break; 
        heap_swap(heap, cur, to_sift);
        cur = to_sift;
    }
}

/**
 * @brief Sift up from binheap algorithm.
 * 
 * @param heap [IN] - heap.
 * @param i - index of node to sift up.
 */
static void
sift_up(BinHeap* heap, size_t cur) {
    while(CMP(heap->data[cur], heap->data[(cur-1) / 2]) < 0) {
        heap_swap(heap, cur, (cur-1) / 2);
        cur = (cur-1) / 2;
    }
}

const heap_elem_t* 
heap_top(const BinHeap* heap) {
    assert(heap != NULL);
    if(heap->size == 0) {
        return NULL;
    }

    assert(heap->data != NULL);
    return &heap->data[0];
}

int heap_extract_top(BinHeap* heap, heap_elem_t* res) {
    assert(heap != NULL);

    if(heap->size == 0) {
        return ERRNOENT;
    }

    assert(heap->data);

    if(res) {
        *res = heap->data[0]; 
    }

    heap_swap(heap, 0, --heap->size);
    sift_down(heap, 0);
    return ERRSUC;
}

int heap_insert(BinHeap* heap, const heap_elem_t* elem) {
    if(heap->size == heap->capacity) {
        heap->data = realloc(heap->data, heap->capacity ?  heap->capacity *= 2 : HEAP_START_CAP);
        assert(heap->data);
    }
    
    heap->data[heap->size++] = *elem;
    sift_up(heap, heap->size - 1);
    return ERRSUC;
}

void 
free_heap(BinHeap* heap) {
    if(!heap) return;
    free_nullify(heap->data);
    heap->size = 0;
}