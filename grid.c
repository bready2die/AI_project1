#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "linux_list.h"
#include "grid.h"
#include "heap.h"
#include "scene.h"
#include "search.h"


static int width, height;
static struct coords start, goal;
static char goal_placed, start_placed, algo_ran;
static char init = 0;

static char* blocks; //grid noting blocked tiles.

//struct list_head closed_list;
struct bstnode* closed_list;
struct heap fringe;

static struct circle start_circle;
static struct circle goal_circle;

static void close_grid();

int new_grid(int _width, int _height)
{
	if (init)
	{
		close_grid();
	}

	if (_width < 1 || _height < 1)
	{
		return 1;
	}
	
	width = _width;
	height = _height;
	goal_placed = 0;
	start_placed = 0;
	algo_ran = 0;
	blocks = malloc(sizeof(char)*width*height);
	memset(blocks, 0, sizeof(char)*width*height);
	
	if (!init)
	{
		closed_list = NULL;
		heap_init(&fringe);
	} else {
		resize_window(_width, _height);
	}
	redraw_scene();

	init = 1;
	return 0;
}

int reset_grid()
{
	return new_grid(width, height);
}

int put_start(int x, int y)
{
	if (x < 1 || x > width+1 || y < 1 || y > height+1)
	{
		return 1;
	}
	clear_vertices();

	start.x = x;
	start.y = y;
	
	//graphics
	struct circle circle = START_CIRCLE( (int)(x), (int)(y), CIRCLE_RAD);
	if (start_placed)
		delcircle(&start_circle);
	memcpy(&start_circle, &circle, sizeof(struct circle));
	addcircle(&start_circle);
	redraw_scene();
	
	start_placed = 1;

	return 0;
}

int get_start(int* x, int* y){
	if (!start_placed)
	{
		return 1;
	}
	*x = start.x;
	*y = start.y;
	return 0;
}

int put_goal(int x, int y)
{
        if (x < 1 || x > width+1 || y < 1 || y > height+1)
        {
                return 1;
        }
	clear_vertices();
        goal.x = x;
        goal.y = y;
        
        //graphics
	struct circle circle = GOAL_CIRCLE( (int)(x), (int)(y), CIRCLE_RAD);
	if (goal_placed)
		delcircle(&goal_circle);
	memcpy(&goal_circle, &circle, sizeof(struct circle));
	addcircle(&circle);
	redraw_scene();
        
	goal_placed = 1;

        return 0;
}

int get_goal(int* x, int* y){
	if (!goal_placed)
	{
		return 1;
	}
	*x = goal.x;
	*y = goal.y;
	return 0;
}

static char redraw_hold = 0;

int set_tile(int x, int y, char block)
{
	if (x < 1 || x > width || y < 1 || y > height)
        {
        	return 1;
        }
        if (algo_ran)
	{
		clear_vertices();
	}
        if (blocks[(x-1)*height+y-1] == block)
        {
        	return 0;
        }
        blocks[(x-1)*height+y-1] = block;
        
        //graphics
        struct rect rect = BLOCK_RECT(x, y);
        if (block)
        {
        	//add block
        	addrect(&rect);

        } else {
        	//remove block
        	delrect(&rect);
        }
        if (!redraw_hold)
        	redraw_scene();

        return 0;
}

int get_tile(int x, int y, char* block)
{
	if (x < 1 || x > width || y < 1 || y > height)
	{
		return 1;
	}
	*block = blocks[(x-1)*height+y-1];
	return 0;
}

int load_file(char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		//invalid file
		return 1;
	}
	
	//Maybe I'll add error handling later
	struct coords startbuf, goalbuf, sizebuf;
	
	fscanf(file, "%d %d ", &(startbuf.x), &(startbuf.y));
	fscanf(file, "%d %d ", &(goalbuf.x), &(goalbuf.y));
	fscanf(file, "%d %d ", &(sizebuf.x), &(sizebuf.y));
	
	new_grid(sizebuf.x, sizebuf.y);
	put_start(startbuf.x, startbuf.y);
	put_goal(goalbuf.x, goalbuf.y);

	int buf1, buf2, buf3;
	redraw_hold = 1;
	while (fscanf(file, "%d %d %d ", &buf1, &buf2, (int*) &buf3) == 3)
	{
		if (buf1 > 0 && buf1 <= width && buf2 > 0 && buf2 <= height)
		{
			set_tile(buf1, buf2, (char)buf3);
		}
	}
	redraw_hold = 0;
	redraw_scene();
	
	if (fclose(file))
	init = 1;
	return 0;
}

int succ(struct coords pos, struct coords* buffer) //returns successor count
{
	int count = 0;

	char flags = 0xf; //flags = 0x1111
	/*	i=0, 0b1000 -> north west	i=1, 0b0100 -> south west
		i=2, 0b0010 -> north east	i=3, 0b0001 -> south east */

	//checks diagonally adjacent tiles
	for (int i = 0; i < 4; i++)
	{
		int cell_x = pos.x + i/2 - 2;
		int cell_y = pos.y + i%2 - 2;
		//checks if tile is empty
		if (cell_x >= 0 && cell_y >= 0 && cell_x < width && cell_y < height 
			&& blocks[cell_x*(height) + cell_y] == 0)
		{
			//unmark flag and add diagonal neighbor
			flags &= ~(1 << (3-i));
			buffer[count] = (struct coords)
			{
				.x=pos.x + (i/2)*2 - 1,
				.y=pos.y + (i%2)*2 - 1
			};
			count++;
		}
	}
	//check cardinal neighbors
	for (int i = 0; i < 2; i++)
	{				//	i=0		i=1
		char xmask = 0xc >> (2*i);//	0b1100 (west)	0b0011 (east)
		char ymask = 0xa >> i;	//	0b1010 (north)	0b0101 (south)

		if (~flags & xmask)// check if either west/east tile is empty
		{
			buffer[count] = (struct coords)//add neighbor
        	        {
			        .x=pos.x + i*2 - 1,
				.y=pos.y
			};
			count++;
		}
		if (~flags & ymask)// check if either north/south tile is empty
                {
                        buffer[count] = (struct coords)//add neighbor
                        {
                                .x=pos.x,
                                .y=pos.y + i*2 - 1
                        };
                        count++;
                }
	}
	/*for (int i = 0; i < count; i++){
		printf("(%d,%d) ", buffer[i].x, buffer[i].y);
	}
	printf("\n");*/
	return count;
}

int init_vertex(int x, int y, struct vertex* buffer)
{
	buffer->position = (struct coords){.x = x, .y = y};
	buffer->g = INT_MAX;
	//buffer->h must be defined later
	buffer->parent = NULL;

	buffer->has_line = 0;

	return 0;
}

static double _seed(struct coords pos){//Number should be practically random in terms of ordering, relative to neighboring coords
	int number = pos.x * height + pos.y;
	return tan(number);
}

static int _add_to_tree(struct vertex* v, double val, struct bstnode** ptr)
{
	if (*ptr == NULL)
	{
		struct bstnode* n = malloc(sizeof(struct bstnode));
		n->vertex = v;
		n->val = _seed(v->position);
		n->left = NULL;
		n->right = NULL;
		*ptr = n;
		return 0;
	}
	if (COORDS_CMP(v->position, (*ptr)->vertex->position))
	{
		return 1;
	}
	if (val < (*ptr)->val)
	{
		return _add_to_tree(v, val, &((*ptr)->left));
	}
	return _add_to_tree(v, val, &((*ptr)->right));
}

int add_to_closed_list (struct vertex* vertex)
{
	return _add_to_tree(vertex, _seed(vertex->position), &closed_list);
}

static int del_closed_list(struct bstnode* root)
{
	if (root == NULL)
		return 1;

	if (root->vertex->has_line)
		delline(&(root->vertex->path_line));
		
	free(root->vertex);
	del_closed_list(root->left);
	del_closed_list(root->right);
	free(root);
	return 0;
}

void clear_vertices ()
{
	if (!algo_ran)
        	return;
        	
        del_closed_list(closed_list);
        closed_list = NULL;
	/*
	struct list_head* i;
	struct list_head* buf;
	struct vertex* v;

	
	list_for_each_safe(i, buf, &closed_list)
	{
		v = list_entry(i, struct vertex, list);
		list_del(i);
		//finish any vertex related business here, maybe clear scene lines
		if (v->has_line)
			delline(&(v->path_line));
		
		free(v);
	}
	*/
	algo_ran = 0;
	heap_destroy(&fringe);
}

static int _search_bst(struct coords coords, double val, struct vertex** output, struct bstnode* root)
{
	if (root == NULL)
	{
		return 1;
	}
	if (COORDS_CMP(coords, root->vertex->position))
	{
		*output = root->vertex;
		return 0;
	}
	if (val < root->val)
	{
		return _search_bst(coords, val, output, root->left);
	}
	return _search_bst(coords, val, output, root->right);
}

int search_closed_list(struct coords coords, struct vertex** output)//TODO
{
	if (!algo_ran)
        	return 1;
	//int test = 1;
	
	return _search_bst(coords, _seed(coords), output, closed_list);
	/*
	struct list_head* i;
	list_for_each(i, &closed_list)
	{
		struct vertex* v = list_entry(i, struct vertex, list);
		if (COORDS_CMP(v->position, coords))
		{
			test = 0;
			*output = v;
			break;
		}
	}
	*/
	//return test;
}


int search_vertices(struct coords coords, struct vertex** output)
{
	int test = heap_search(&fringe, coords, output);
	if (test)
	{
		test = search_closed_list(coords, output);
	}
	return test;
}

int get_hval(int x, int y, double* ret)
{
	if (!algo_ran)
        	return 1;
	struct vertex* target;
	int test = search_vertices((struct coords){.x=x,.y=y}, &target);
	if (test == 1)
	{
		return 1;
	}

	*ret = target->h;
	return 0;
}

int get_fval(int x, int y, double* ret)
{
        if (!algo_ran)
        	return 1;
        struct vertex* target;
        int test = search_vertices((struct coords){.x=x,.y=y}, &target);
	if (test == 1)
	{
		return 1;
	}

        *ret = target->h + target->g;
        return 0;
}

int run_algo(char* type, double* total_cost)
{

	clear_vertices();
	algo_ran = type[0];
	return search(total_cost, algo_ran);
	
}

int make_path(struct vertex* goal)
{
	struct vertex* ptr = goal;
	while (1)
	{
		if (!(ptr->parent) || COORDS_CMP(ptr->parent->position, ptr->position) )
		{
			break;
		}
		struct line line = PATH_LINE(ptr->position.x, ptr->position.y, 
					ptr->parent->position.x, ptr->parent->position.y);
		if (ptr->has_line)
			delline(&(ptr->path_line));
		memcpy(&(ptr->path_line), &line, sizeof(struct line));
		addline(&(ptr->path_line));

		ptr->has_line = 1;
	
		ptr = ptr->parent;
	}
	
	redraw_scene();
	return 0;
}

static void close_grid()
{
	redraw_hold = 1;
	for (int i = 0; i < width*height; i++)
	{
		set_tile(i/height+1, i%height+1, 0);
	}
	redraw_hold = 0;
	free(blocks);
	if (goal_placed)
		delcircle(&goal_circle);
	if (start_placed)
		delcircle(&start_circle);
	clear_vertices();
}


