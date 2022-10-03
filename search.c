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
	if (alg == 't')
	{
		double xdist = abs(position.x - goalpos.x);
		double ydist = abs(position.y - goalpos.y);
		return sqrt(xdist*xdist + ydist*ydist);
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
		//list_add(&(s->list), &closed_list);
		add_to_closed_list (s);
		if (COORDS_CMP(s->position, goalpos))
		{
			*path_cost = s->g;
			make_path(s);
			return 0;
		}
		//make_path(s);//
		
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
					neighbor->h = h(neighbor->position, alg);
				}
				update_vertex(s, neighbor, alg);
			} 
		}
	}
	
	return 1;
}

static int line_of_sight(struct coords p1, struct coords p2)
{
	int x0 = p1.x; int y0 = p1.y; int x1 = p2.x; int y1 = p2.y;
	int f = 0; int dy = y1 - y0; int dx = x1 - x0;
	int sy, sx; char block, block2;
	if (dy < 0)
	{
		dy = -dy; sy = -1;
	} else {
		sy = 1;
	}
	if (dx < 0)
	{
		dx = -dx; sx = -1;
	} else {
		sx = 1;
	}
	if (dx >= dy)
	{
		while (x0 != x1)
		{
			f = f + dy;
			if (f >= dx)
			{
				get_tile(x0 + ((sx - 1)/2), y0 + ((sy - 1)/2), &block);
				if (block)
				{
					return 0;
				}
				y0 = y0 + sy;
				f = f - dx;
			}
			get_tile(x0 + ((sx - 1)/2), y0 + ((sy - 1)/2), &block);
			if (f != 0 && block)
			{
				return 0;
			}
			get_tile(x0 + ((sx - 1)/2), y0, &block);
			get_tile(x0 + ((sx - 1)/2), y0 - 1, &block2);
			if (dy == 0 && block && block2)
			{
				return 0;
			}
			x0 = x0 + sx;
		} 
	} else {
		while (y0 != y1)
		{
			f = f + dx;
			if (f >= dy)
			{
				get_tile(x0 + ((sx - 1)/2), y0 + ((sy - 1)/2), &block);
				if (block)
				{
					return 0;
				}
				x0 = x0 + sx;
				f = f - dy;
			}
			get_tile(x0 + ((sx - 1)/2), y0 + ((sy - 1)/2), &block);
			if (f != 0 && block)
			{
				return 0;
			}
			get_tile(x0, y0 + ((sy - 1)/2), &block);
			get_tile(x0 - 1, y0 + ((sy - 1)/2), &block2);
			if (dx == 0 && block && block2)
			{
				return 0;
			}
			y0 = y0 + sy;
		} 
	}
	return 1;
}

static int update_vertex(struct vertex* s, struct vertex* s2, char alg)
{
	if (alg == 't' && line_of_sight(s->parent->position, s2->position))
	{
		if (s->parent->g + c(s->parent->position, s2->position) < s2->g)
		{
			s2->g = s->parent->g + c(s->parent->position, s2->position);
			s2->parent = s->parent;
			if (!heap_search(&fringe, s2->position, NULL))
			{
				heap_remove(&fringe, s2->position, &s2);
			}
			heap_insert(&fringe, s2, s2->g+s2->h);
		}
	} 
	else if (s->g + c(s->position, s2->position) < s2->g)
	{
		s2->g = s->g + c(s->position, s2->position);
		s2->parent = s;
		if (!heap_search(&fringe, s2->position, NULL))
		{
			heap_remove(&fringe, s2->position, &s2);
		}
		heap_insert(&fringe, s2, s2->g+s2->h);
	}
	return 0;
}
