#ifndef SCENE_LIB
#define SCENE_LIB 1
#include "linux_list.h"

#define GOAL_R 0
#define GOAL_G 255
#define GOAL_B 0

#define START_R 0
#define START_G 0
#define START_B 255

#define GRID_R 255
#define GRID_G 0
#define GRID_B 0

#define BLOCK_R 0
#define BLOCK_G 0
#define BLOCK_B 0

#define PATH_R 255
#define PATH_G 0
#define PATH_B 255

#define _COLOR_SHAPE(__shape, __r,__g,__b)	\
do {						\
	__shape.r = (__r);			\
	__shape.g = (__g);			\
	__shape.b = (__b);			\
} while (0)
	


#define _MAKE_CIRCLE(__name,__x,__y,__rad)	\
	struct circle (__name) = {		\
		.x = (__x),			\
		.y = (__y),			\
		.rad = (__rad),			\
	}

				
#define GOAL_CIRCLE( _x, _y, _rad)				\
({								\	                
	_MAKE_CIRCLE(__temp_circle,(_x),(_y),(_rad));		\
	_COLOR_SHAPE(__temp_circle,GOAL_R,GOAL_G,GOAL_B);	\
	__temp_circle;						\
})

#define START_CIRCLE( _x, _y, _rad)				\
({								\	                
	_MAKE_CIRCLE(__temp_circle,(_x),(_y),(_rad));		\
	_COLOR_SHAPE(__temp_circle,START_R,START_G,START_B);	\
	__temp_circle;						\
})



#define _MAKE_LINE(__name, __x1, __y1, __x2, __y2)      \
	struct line (__name) = {			\
		  .x1 = (__x1),				\
		  .y1 = (__y1),				\
		  .x2 = (__x2),				\
		  .y2 = (__y2),				\
	  }

#define BLOCKING_LINE(_x1, _y1, _x2, _y2)			\
({								\
	_MAKE_LINE(__temp_line,(_x1),(_y1),(_x2),(_y2));	\
	_COLOR_SHAPE(__temp_line,BLOCK_R,BLOCK_G,BLOCK_B);	\
	__temp_line;						\
})

#define PATH_LINE(_x1, _y1, _x2, _y2)				\
({								\
	_MAKE_LINE(__temp_line,(_x1),(_y1),(_x2),(_y2));	\
	_COLOR_SHAPE(__temp_line,PATH_R,PATH_G,PATH_B);		\
	__temp_line;						\
})

#define GRID_LINE(_x1, _y1, _x2, _y2)				\
({								\
	_MAKE_LINE(__temp_line,(_x1),(_y1),(_x2),(_y2));	\
	_COLOR_SHAPE(__temp_line,GRID_R,GRID_G,GRID_B);		\
	__temp_line;						\
})


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
