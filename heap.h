#ifndef PROJ_HEAP
#define PROJ_HEAP

#include "grid.h"

struct node {
	struct vertex* element;
	double value;
};

struct heap {
	struct node** array;
	int size;
	int max_size;
};

int heap_init(struct heap* new_heap);

int heap_insert(struct heap* h, struct vertex* element, double value);

int heap_pop(struct heap* h, struct vertex** output);

int heap_search(struct heap* h, struct coords position, struct vertex** output);

int heap_remove(struct heap* h, struct coords position, struct vertex** output);

int heap_is_empty(struct heap* h);

int heap_destroy(struct heap* h);

int heap_debug_print(struct heap* h);

#endif
