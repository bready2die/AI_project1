#include <stdlib.h>
#include <math.h>
#include "grid.h"

static struct coords startpos;
static struct coords goalpos;

static double h(struct coords position, char alg)
{
	if (alg == 'a')
	{
		double xdist = abs(position.x - goalpos.x);
		double ydist = abs(position.y - goalpos.y);
		double max = fmax(xdist, ydist);
		double min = fmin(xdist, ydist);
		return max - min + 1.41421356237 * min;
	}
	return 0;
}

static double c(struct coords p1, struct coords p2)
{
	double xdist = abs(p1.x - p2.x);
	double ydist = abs(p1.y - p2.y);
	return sqrt(xdist*xdist + ydist*ydist);
}

static int update_vertex(struct vertex* s, struct vertex* s2, char alg);

int search(double* path_cost, char alg)
{
	struct vertex* start = malloc(sizeof(struct vertex));
	get_start(&startpos.x, &startpos.y);
	get_goal(&goalpos.x, &goalpos.y);
	init_vertex(startpos.x, startpos.y, start);
	start->parent = start;
	start->g = 0;
	start->h = h(start->position, alg);
	heap_insert(&fringe, start, start->h + start->g);
	
	while (!heap_is_empty(&fringe))
	{
		struct vertex* s;
		heap_pop(&fringe, &s); 
		list_add(&(s->list), &closed_list);
		if (COORDS_CMP(s->position, goalpos))
		{
			*path_cost = s->g;
			make_path(s);
			return 0;
		}
		
		struct coords buffer[8];
		int count = succ(s->position, buffer);
		for(int i = 0; i < count; i++)
		{
			struct coords s2 = buffer[i];
			struct vertex* neighbor;
			if (search_closed_list(s2, &neighbor))
			{
				if (heap_search(&fringe, s2, &neighbor))// return 1 = "error" ie not found
				{
					neighbor = malloc(sizeof(struct vertex));
					init_vertex(s2.x, s2.y, neighbor);
				}
				update_vertex(s, neighbor, alg);
			} 
		}
	}
	
	return 1;
}

static int update_vertex(struct vertex* s, struct vertex* s2, char alg)
{
	if (s->g + c(s->position, s2->position) < s2->g)
	{
		s2->g = s->g + c(s->position, s2->position);
		s2->parent = s;
		if (!heap_search(&fringe, s2->position, NULL))
		{
			heap_remove(&fringe, s2->position, NULL);
		}
		heap_insert(&fringe, s2, s2->g+s2->h);
	}
	return 0;
}
