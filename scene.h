#ifndef SCENE_LIB
#define SCENE_LIB 1
#include "linux_list.h"


struct point {
		struct list_head list;
		union {
				struct {
						int x;
						int y;
						char r;
						char g;
						char b;
				}__attribute__((packed));;
				char bytes[11];
		};
};

struct line {
		struct list_head list;
		union {
				struct {
						int x1;
						int y1;
						int x2;
						int y2;
						char r;
						char g;
						char b;
				}__attribute__((packed));;
				char bytes[19];
		};
};

struct circle {
		struct list_head list;
		union {
				struct {
						int x;
						int y;
						int rad;
						char r;
						char g;
						char b;
				}__attribute__((packed));
				char bytes[15];
		};
};


//currently unused, may be useful in case the graph model needs 
//to take a deeper look at the screen context for whatever reason
//no methods for passing it over yet, though
struct scene_context {
		struct list_head scene_lines;
		struct list_head scene_points;
		struct list_head scene_circles;
};

int addline(struct line *line);

int delline(struct line *line);

int addpoint(struct point *point);

int delpoint(struct point *point);

int addcircle(struct circle *circle);

int delcircle(struct circle *circle);

int start_scene();

int redraw_scene();
#endif
