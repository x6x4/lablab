#pragma once

#include <stddef.h>
#include <stdio.h>

typedef struct{ssize_t dist; size_t vert;} heap_elem_t;

#define CMP(a, b) ((a).dist - (b).dist) 

struct BinHeap {
    heap_elem_t *data;
    size_t size;
    size_t capacity;
};

typedef struct BinHeap BinHeap;

const heap_elem_t* heap_top(const BinHeap* heap);

int heap_extract_top(BinHeap* heap, heap_elem_t* res);

int heap_insert(BinHeap* heap, const heap_elem_t* elem);

void free_heap(BinHeap* heap);