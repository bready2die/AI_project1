#ifndef PROJ_HEAP
#define PROJ_HEAP

#include "grid.h"

struct node {
	void* element;
	float value;
};

struct heap {
	struct node** array;
	int size;
	int max_size;
};

int heap_init(struct heap* new_heap);

int heap_insert(struct heap* h, void* element, float value);

void* heap_pop(struct heap* h);

int heap_search(struct heap* h, void* element);

int heap_remove(struct heap* h, void* element);

int heap_is_empty(struct heap* h);

int heap_destroy(struct heap* h);

int heap_debug_print(struct heap* h);

#endif
