#ifndef GRID_H
#define GRID_H 1

#include "linux_list.h"
#include "scene.h"
#include "heap.h"

struct bstnode;

extern struct bstnode* closed_list;
extern struct heap fringe;

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
        //struct list_head list;
        
        //store lines for scene here? (this to parent, most likely)
        char has_line;
        struct line path_line;
};

struct bstnode 
{
	struct vertex* vertex;
	double val;
	struct bstnode* left;
	struct bstnode* right;
};

int new_grid(int _width, int _height); // makes a new grid, replacing any old one

int reset_grid();//new_grid, but keeps the same width and height

int put_start(int x, int y);

int get_start(int* x, int* y);

int put_goal(int x, int y);

int get_goal(int* x, int* y);

int set_tile(int x, int y, char block); // set tile status: 0 for unblocked, 1 for blocked

int get_tile(int x, int y, char* block);

int load_file(char* filename);

int succ(struct coords pos, struct coords* buffer); //returns successor count. All output coords are stored to buffer.

int init_vertex(int x, int y, struct vertex* buffer);//NOTE: the .h value must be explicitly initialized afterwards, as it is dependent on the algorithm.

int add_to_closed_list (struct vertex* vertex);

void clear_vertices(); //removes all vertex and path information, effectively undos search

int search_closed_list(struct coords coords, struct vertex** output);

int search_vertices(struct coords coords, struct vertex** output);

int get_hval(int x, int y, double* ret);

int get_gval(int x, int y, double* ret);

int get_fval(int x, int y, double* ret);

int run_algo(char* type, double* total_cost);

int make_path(struct vertex* goal);
//run this one on the goal to create a path ending at the goal



#endif
