#include <stdlib.h>
#include <stfio.h>

struct grid
{
	int width, height, startX, startY, goalX, goalY;
	char* blocks; //grid noting blocked tiles. 
	//path data may or may not be stored here, tbd
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

void close_grid(struct grid* grid)//note:does not free the pointer itself
{
	free(grid->blocks);
}


