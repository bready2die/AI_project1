#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linux_list.h"

//NOTE: NONE of this has been bug tested.

static int width, height;
static struct coords start, goal;
static char goal_placed, start_placed, algo_ran;
static char init = 0;
static char* blocks; //grid noting blocked tiles.
static struct list_head closed_list;


struct coords
{
	int x, y;
};

struct vertex // this setup may change, maybe
{
	struct coords position;
	double g; //distance from start, following path
	double h; //heuristic (estimated distance from vertex to goal)
	struct vertex* parent;
	struct list_head list;

	//store lines for scene here? (this to parent, most likely)
}

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

	init = 1;
	return 0;
}

int put_start(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return 1;
	}
	start.x = x;
	start.y = y;
	start_placed = 1;

	return 0;
}

int put_goal(int x, int y)
{
        if (x < 0 || x >= width || y < 0 || y >= height)
        {
                return 1;
        }
        goal.x = x;
        goal.y = y;
	goal_placed = 1;

        return 0;
}

int load_file(char* filename)
{
	if (!init)
        {
                close_grid();
        }

	//INIT new struct grid
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		//invalid file
		return 1;
	}
	//int count; Maybe I'll add error handling later
	fscanf(file, "%d %d ", &(start.x), &(start.y));
	fscanf(file, "%d %d ", &(goal.x), &(goal.y));
	fscanf(file, "%d %d ", &(width), &(height));
	
	goal_placed = 1;
	start_placed = 1;
	algo_ran = 0;

	blocks = malloc(sizeof(char)*width*height);
	
	int buf1, buf2;
	char buf3;
	while (fscanf("%d %d %d ", &buf1, &buf2, &buf3) == 3)
	{
		if (buf1 > 0 && buf1 <= width || buf2 > 0 && buf2 <= height)
		{
			buf1--;
			buf2--;
			blocks[buf1*height + buf2] = buf3;
		}
	}

	//init start vertex, I guess?
	LIST_HEAD(closed_list);

	if (fclose(file))
	init = 1;
	return 0;
}

static int succ(struct coords pos, struct coords* buffer) //returns successor count
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
			//unmark flag and add diagonal neughbor
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
		free(v);
	}
	//TODO
}

void close_grid()//note:does not free the pointer itself
{
	free(blocks);
	clear_vertices();
}


