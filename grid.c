#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linux_list.h"
#include "grid.h"
#include "heap.h"
#include "scene.h"


static int width, height;
static struct coords start, goal;
static char goal_placed, start_placed, algo_ran;
static char init = 0;

static char* blocks; //grid noting blocked tiles.

static struct list_head closed_list;
static struct heap fringe;

static struct circle start_circle;
static struct circle goal_circle;

static void close_grid();

int new_grid(int _width, int _height)
{
	if (!init)
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
	memset(blocks, 0, width*height);
	LIST_HEAD(closed_list);
	heap_init(&fringe);
	
	resize_window(_width, _height);
	redraw_scene();

	init = 1;
	return 0;
}

int put_start(int x, int y)
{
	if (x < 1 || x > width || y < 1 || y > height)
	{
		return 1;
	}
	start.x = x;
	start.y = y;
	
	//graphics
	struct circle circle = START_CIRCLE( (int)(x-1), (int)(y-1), CIRCLE_RAD);
	if (goal_placed)
		delcircle(&start_circle);
	memcpy(&start_circle, &circle, sizeof(struct circle));
	addcircle(&circle);
	redraw_scene();
	
	start_placed = 1;

	return 0;
}

int put_goal(int x, int y)
{
        if (x < 1 || x > width || y < 1 || y > height)
        {
                return 1;
        }
        goal.x = x;
        goal.y = y;
        
        //graphics
	struct circle circle = GOAL_CIRCLE( (int)(x-1), (int)(y-1), CIRCLE_RAD);
	if (goal_placed)
		delcircle(&goal_circle);
	memcpy(&goal_circle, &circle, sizeof(struct circle));
	addcircle(&circle);
	redraw_scene();
        
	goal_placed = 1;

        return 0;
}

int set_tile(int x, int y, char block){
	if (x < 1 || x >= width || y < 1 || y >= height)
        {
        	return 1;
        }
        if (blocks[(x-1)*height+y-1] == block)
        {
        	return 2;
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

	int buf1, buf2;
	char buf3;
	while (fscanf(file, "%d %d %d ", &buf1, &buf2, (int*) &buf3) == 3)
	{
		if (buf1 > 0 && buf1 <= width && buf2 > 0 && buf2 <= height)
		{
			set_tile(buf1, buf2, buf3);
		}
	}

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
		int cell_x = pos.x + i/2 - 1;
		int cell_y = pos.y + i%2 - 1;
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
	return count;
}

void clear_vertices ()
{
	struct list_head* i;
	struct list_head* buf;
	struct vertex* v;

	list_for_each_safe(i, buf, &closed_list)
	{
		v = list_entry(i, struct vertex, list);
		list_del(i);
		//finish any vertex related business here, maybe clear scene lines
		delline(&(v->path_line)); ///seems iffy, considering rogue data
		
		free(v);
	}
	
	algo_ran = 0;
	//TODO
}

static int search_vertices(struct coords coords, struct vertex** output)
{
	int test = heap_search(&fringe, coords, output);
	if (test)
	{
		struct list_head* i;
		list_for_each(i, &closed_list)
		{
			struct vertex* v = list_entry(i, struct vertex, list);
			if (COORDS_CMP((*output)->position, coords))
			{
				test = 0;
				*output = v;
				break;
			}
		}
	}
	if (!test)
	{
		return 1;
	}
	return 0;
}

int get_hval(int x, int y, double* ret)
{
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
        struct vertex* target;
        int test = search_vertices((struct coords){.x=x,.y=y}, &target);
	if (test == 1)
	{
		return 1;
	}

        *ret = target->h + target->g;
        return 0;
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
		memcpy(&(ptr->path_line), &line, sizeof(struct line));
		addline(&(ptr->path_line));
	
		ptr = ptr->parent;
	}
	
	redraw_scene();
	return 0;
}

static void close_grid()//note:does not free the pointer itself
{
	for (int i = 0; i < width*height; i++)
	{
		set_tile(i/height+1, i%height+1, 0);
	}
	free(blocks);
	if (goal_placed)
		delcircle(&goal_circle);
	if (start_placed)
		delcircle(&start_circle);
	
	clear_vertices();
	heap_destroy(&fringe);
}


