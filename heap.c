/*
	NOTE: Intended for use in the algorithms
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "heap.h"

static void swap(struct node** n1, struct node** n2){
	struct node* temp = *n1;
	*n1 = *n2;
	*n2 = temp;
}

static void upheap(struct heap* h, int index){
	for (int i = index; i != 0 && h->array[i]->value < h->array[(i-1)/2]->value; i = (i-1)/2){
                swap(&h->array[i], &h->array[(i-1)/2]);
		/*struct node* temp = h->array[i];
                h->array[i] = h->array[(i-1)/2];
                h->array[(i-1)/2] = temp;*/
        }
}

static void downheap(struct heap* h, int index){
	int i = index;
	while (1){
                if (i*2+2 < h->size && h->array[i]->value > h->array[i*2+2]->value && h->array[i*2+1]->value > h->array[i*2+2]->value){
                        swap(&h->array[i], &h->array[i*2+2]);
			/*struct node* temp = h->array[i];
                        h->array[i] = h->array[i*2+2];
                        h->array[i*2+2] = temp;*/
                        i = i*2+2;
                        continue;
                }
                if (i*2+1 < h->size && h->array[i]->value > h->array[i*2+1]->value){
			swap(&h->array[i], &h->array[i*2+1]);
                        /*struct node* temp = h->array[i];
                        h->array[i] = h->array[i*2+1];
                        h->array[i*2+1] = temp;*/
                        i = i*2+1;
			continue;
                }
		break;
        }
}

int heap_init(struct heap* new_heap){
	struct heap buffer = {
		.array = malloc(sizeof(struct vertex*)),
		.size = 0,
		.max_size = 1
	};
	*new_heap = buffer;
	memset(new_heap->array, 0, sizeof(struct vertex*)*new_heap->max_size);
	return 0;
}

int heap_insert(struct heap* h, struct vertex* element, double value){
	//extend heap if necessary
	if (h->size >= h->max_size){
		h->max_size *= 2;
		struct node** new_array = realloc(h->array, h->max_size * sizeof(struct node));
		if (new_array == NULL){
			return 1;//resize fail
		}
		h->array = new_array;
	}

	struct node* new_node = malloc(sizeof(struct node));
	struct node buffer = {
		.element = element,
		.value = value
	};
	*new_node = buffer;
	h->array[h->size] = new_node;
	h->size++;

	upheap(h, h->size - 1);

	return 0;
}

int heap_pop(struct heap* h, struct vertex** output){
	if (h->size <= 0)
	{
		return 1;
	}

	struct vertex* out = h->array[0]->element;
	free(h->array[0]);
	h->array[0] = h->array[h->size-1];
	h->size -= 1;

	downheap(h, 0);

	*output = out;
	return 0;
}

int heap_search(struct heap* h, struct coords position, struct vertex** output){
	for (int i = 0; i < h->size; i++){
		if (COORDS_CMP(h->array[i]->element->position, position)){
			if (output)
			{
				*output = h->array[i]->element;
			}
			return 0;
		}
	}
	return 1;
}

int heap_remove(struct heap* h, struct coords position, struct vertex** output){
        for (int i = 0; i < h->size; i++){
		if (COORDS_CMP(h->array[i]->element->position, position)){
			swap(&h->array[i], &h->array[h->size - 1]);
			if (h->array[i]->value < h->array[h->size-1]->value){
				upheap(h, i);
			} else {
				downheap(h, i);
			}

			if (output)
			{
				*output = h->array[h->size-1]->element;
			}
			free(h->array[h->size-1]);
			h->size -= 1;
			return 0;
		}
	}
	return 1;
}

int heap_is_empty(struct heap* h){
	return h->size == 0;
}

int heap_destroy(struct heap* h){//NOTE FREES ALL VERTICES
	for (int i = 0; i < h->size; i++){
		free(h->array[0]->element);
		free(h->array[0]);
	}
	free(h->array);
	return 0;
}

int heap_debug_print(struct heap* h){
	for (int i = 0; i < h->size; i++){
                printf("%f ", h->array[i]->value);
        }
	printf("\n");
	return 0;
}
