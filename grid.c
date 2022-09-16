#include <stdlib.h>
#include <stfio.h>

struct grid
{
	int width, height, startX, startY, goalX, goalY;
	char* blocks; //grid noting blocked tiles. 
	//path data may or may not be stored here, tbd
}

struct coords
{
	int x, y;
};

struct vertex // this setup may change, maybe
{
	struct coords;
	float g; //distance from start, following path
	float h; //heuristic (estimated distance from vertex to goal)
	struct vertex* parent;
}

int read_file(struct grid* grid, char* filename)
{
	//INIT new struct grid
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		//invalid file
		return 1;
	}
	//int count; Maybe I'll add error handling later
	fscanf(file, "%d %d ", &(grid->startX), &(grid->startY));
	fscanf(file, "%d %d ", &(grid->goalX), &(grid->goalY));
	fscanf(file, "%d %d ", &(grid->width), &(grid->height));
	
	grid->blocks = malloc(sizeof(char)*grid->width*grid->height);
	
	int buf1, buf2;
	char buf3;
	while (fscanf("%d %d %d ", &buf1, &buf2, &buf3) == 3)
	{
		if (buf1 > 0 && buf1 <= grid->width || buf2 > 0 && buf2 <= grid->height)
		{
			buf1--;
			buf2--;
			grid->blocks[buf1*(grid->height) + buf2] = buf3;
		}
	}

	if (fclose(file))
	return 0;
}

static int succ(struct grid* grid, struct coords pos, struct coords* buffer) //returns successor count
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
		if (cell_x >= 0 && cell_y >= 0 && cell_x < grid->width && cell_y < grid->height 
			&& grid->blocks[cell_x*(grid->height) + cell_y] == 0)
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

void close_grid(struct grid* grid)//note:does not free the pointer itself
{
	free(grid->blocks);
}


