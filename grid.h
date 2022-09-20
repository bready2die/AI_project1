#ifndef GRID_H
#define GRID_H 1

#include "linux_list.h"

struct coords
{
        int x, y;
};

#define COORDS_CMP(a, b)\
	((a).x == (b).x && (a).y == (b).y)

struct vertex // this setup may change, maybe
{
        struct coords position;
        double g; //distance from start, following path
        double h; //heuristic (estimated distance from vertex to goal)
        struct vertex* parent;
        struct list_head list;
        
        //store lines for scene here? (this to parent, most likely)
        struct line path_line;
};

int new_grid(int _width, int _height);

int put_start(int x, int y);

int put_goal(int x, int y);

int set_tile(int x, int y, char block);

int load_file(char* filename);

void clear_vertices();

int get_hval(int x, int y, double* ret);

int get_fval(int x, int y, double* ret);

int make_path(struct vertex* goal);
//run this one on the goal to create a path ending at the goal

static void close_grid();//note:does not free the pointer itself

#endif
