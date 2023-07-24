#include "lib_bin_heap.h"
#include "generic.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


const HEntry *heap_get_top (const BHeap *heap) {

    assert (heap);

    if (heap->sz == 0)
        return NULL;

    assert (heap->data);

    return heap->data;
}

int heap_extract_top (BHeap *heap, HEntry *top) {

    assert (heap);

    if (heap->sz == 0)
        return ERRUNF;

    assert (heap->data);

    if (top) *top = heap->data[0];

    heap_swap (heap, 0, --heap->sz);
    fix_mid (heap);

    return ERRSUC;
}

void heap_insert (BHeap *heap, const HEntry *entry) {

    //  handle overflow
    if (heap->sz == heap->capacity) {

        size_t new_cap = heap->capacity ? heap->capacity *=2 : HEAP_START_CAP;
        heap->capacity = new_cap;

        heap->data = reallocarray (heap->data, heap->capacity, sizeof *(heap->data));
    
        assert (heap->data);
    }

    //  usual insertion
    heap->data[heap->sz++] = *entry;

    fix_end (heap);

    return;
}

void fix_end (BHeap *heap) {

    size_t cur_end = heap->sz - 1;

    while (cur_end && GT (heap->data[MID(cur_end)], heap->data[cur_end])) {
        
        //  balance
        heap_swap (heap, cur_end, MID(cur_end));

        //  move left
        cur_end = MID(cur_end);
    }
}

void fix_mid (BHeap *heap) {

    size_t cur_mid = 0;

    while (END1(cur_mid) < heap->sz) {

        size_t end = least_end (heap, cur_mid);

        //  end: heap is balanced, end > mid
        if (GT(heap->data[end], heap->data[cur_mid])) break;

        else { 
            
            //  balance
            heap_swap (heap, cur_mid, end);

            //  move right
            cur_mid = end;
        }
    }
}

size_t least_end (BHeap *heap, const size_t mid) {

    size_t end = END1(mid);
    size_t end2 = end + 1; 

    if (end2 < heap->sz)
        if (GT (heap->data[end], heap->data[end2])) 
            end = end2;

    return end;
}

void heap_swap (BHeap *heap, size_t lhs, size_t rhs) {

    assert (heap);

    struct heap_entry tmp = heap->data[rhs];
    heap->data[rhs] = heap->data[lhs];
    heap->data[lhs] = tmp;
}

void heap_free (BHeap *heap) {

    if (!heap) return;

    free_nullify (heap->data);

    heap->sz = 0;
    heap->capacity = 0;
}

void heap_print (const BHeap heap) {

    for (size_t i = 0; i < heap.sz; i++) 
        printf ("%ld ", heap.data[i].dist);

    printf ("\n");
}