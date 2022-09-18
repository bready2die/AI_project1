#ifndef GRID_H
#define GRID_H 1

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
}

#endif
