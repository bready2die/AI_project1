#include <stdlib.h>
#include <math.h>
#include "grid.h"

static struct coords startpos;
static struct coords goalpos;

static double h(struct coords position, char alg)
{
	if (alg = 'a')
	{
		double xdist = abs(position.x - goalpos.x);
		double ydist = abs(position.y - goalpos.y);
		double max = fmax(xdist, ydist);
		double min = fmin(xdist, ydist);
		return max - min + 1.41421356237 * min;
	}
	return 0;
}

int search(double* path_cost, char alg)
{
	struct vertex* start = malloc(sizeof(struct vertex));
	get_start(&startpos.x, &startpos.y);
	get_goal(&goalpos.x, &goalpos.y);
	init_vertex(startpos.x, startpos.y, start);
	start->parent = start;
	start->g = 0;
	start->h = h(start->position, alg);
	heap_insert(fringe, start, start->h + start->g);
	
	while (!heap_is_empty(fringe))
	{
		struct vertex* s;
		heap_pop(fringe, &s); 
		if (COORDS_CMP(s->position, goalpos))
		{
			*path_cost = s->g;
			return 0;
		}
		list_add(&(s->list), &closed_list);
		
		struct coords buffer[8];
		int count = succ(s->position, &buffer);
		//TODO
	}
	

	return 1;
}
