#ifndef SCENE_LIB
#define SCENE_LIB 1
#include "linux_list.h"


extern int grid_width;

extern int grid_height;


//THESE MUST BE MOVED INTO grid.h/////////////
#define MAX_GRID_HEIGHT 100

#define MAX_GRID_WIDTH 50

#define DEF_GRID_HEIGHT MAX_GRID_HEIGHT
#define DEF_GRID_WIDTH MAX_GRID_WIDTH
//////////////////////////////////////////////




#define GRID_MWIDTH ( grid_width + 1)

#define GRID_MHEIGHT ( grid_height + 1)

#define WIN_BORDER 5

#define PX_PER_SPACE 10 

#define WINHEIGHT ((2 * (WIN_BORDER)) + (PX_PER_SPACE) * grid_height)
#define WINWIDTH ((2 * (WIN_BORDER)) + (PX_PER_SPACE) * grid_width)

#define CIRCLE_RAD 4

#define BG_R 127
#define BG_G 127
#define BG_B 127

#define GOAL_R 0
#define GOAL_G 255
#define GOAL_B 0

#define START_R 0
#define START_G 255
#define START_B 255

#define GRID_R 63
#define GRID_G 0
#define GRID_B 0

#define BLOCK_R 31
#define BLOCK_G 31
#define BLOCK_B 31

#define PATH_R 255
#define PATH_G 0
#define PATH_B 255

#define DIAG_R 127
#define DIAG_G 90
#define DIAG_B 90

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
	_MAKE_CIRCLE(___temp_circle,(_x) - 1,(_y) - 1,(_rad));	\
	_COLOR_SHAPE(___temp_circle,GOAL_R,GOAL_G,GOAL_B);	\
	___temp_circle;						\
})

#define START_CIRCLE( _x, _y, _rad)				\
({								\
	_MAKE_CIRCLE(___temp_circle,(_x) - 1 ,(_y) - 1,(_rad));	\
	_COLOR_SHAPE(___temp_circle,START_R,START_G,START_B);	\
	___temp_circle;						\
})



#define _MAKE_LINE(__name, __x1, __y1, __x2, __y2)      \
	struct line (__name) = {			\
		  .x1 = (__x1),				\
		  .y1 = (__y1),				\
		  .x2 = (__x2),				\
		  .y2 = (__y2),				\
	  }

#define BLOCKING_LINE(_x1, _y1, _x2, _y2)					\
({										\
	_MAKE_LINE(___temp_line,(_x1) - 1,(_y1) - 1,(_x2) - 1,(_y2) - 1);	\
	_COLOR_SHAPE(___temp_line,BLOCK_R,BLOCK_G,BLOCK_B);			\
	___temp_line;								\
})

#define PATH_LINE(_x1, _y1, _x2, _y2)						\
({										\
	_MAKE_LINE(___temp_line,(_x1) - 1,(_y1) - 1,(_x2) - 1,(_y2) - 1);	\
	_COLOR_SHAPE(___temp_line,PATH_R,PATH_G,PATH_B);			\
	___temp_line;								\
})

#define GRID_LINE(_x1, _y1, _x2, _y2)				\
({								\
	_MAKE_LINE(___temp_line,(_x1),(_y1),(_x2),(_y2));	\
	_COLOR_SHAPE(___temp_line,GRID_R,GRID_G,GRID_B);	\
	___temp_line;						\
})

#define DIAG_LINE(_x1, _y1, _x2, _y2)				\
({								\
	_MAKE_LINE(___temp_line,(_x1),(_y1),(_x2),(_y2));	\
	_COLOR_SHAPE(___temp_line,DIAG_R,DIAG_G,DIAG_B);	\
	___temp_line;						\
})

#define _MAKE_RECT(__name,__x,__y,__w,__h)	\
	struct rect (__name) = {		\
		.x = (__x),			\
		.y = (__y),			\
		.w = (__w),			\
		.h = (__h),			\
	}

#define BLOCK_RECT(_x,_y)					\
({								\
	_MAKE_RECT(___temp_rect,(_x) - 1,(_y) - 1,1,1);		\
	_COLOR_SHAPE(___temp_rect,BLOCK_R,BLOCK_G,BLOCK_B);	\
	___temp_rect;						\
})

#define _RECT_DATA				\
	struct {				\
			int x;			\
			int y;			\
			int w;			\
			int h;			\
			char r;			\
			char g;			\
			char b;			\
	}__attribute__((packed))
		
struct rect {
	struct list_head list;
	union {
		_RECT_DATA;
		char bytes[sizeof(_RECT_DATA)];
	};
};


#define _POINT_DATA				\
	struct {				\
		int x;				\
		int y;				\
		char r;				\
		char g;				\
		char b;				\
	}__attribute__((packed))


struct point {
	struct list_head list;
	union {
	        _POINT_DATA;
		char bytes[sizeof(_POINT_DATA)];
	};
};

#define _LINE_DATA			\
	struct {			\
		int x1;			\
		int y1;			\
		int x2;			\
		int y2;			\
		char r;			\
		char g;			\
		char b;			\
	}__attribute__((packed))

struct line {
	struct list_head list;
	union {
	        _LINE_DATA;
		char bytes[sizeof(_LINE_DATA)];
	};
};


#define _CIRCLE_DATA			\
	struct {			\
		int x;			\
		int y;			\
		int rad;		\
		char r;			\
		char g;			\
		char b;			\
	}__attribute__((packed))	
	
struct circle {
	struct list_head list;
	union {
		_CIRCLE_DATA;
		char bytes[sizeof(_CIRCLE_DATA)];
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


int addrect(struct rect *rect);

int delrect(struct rect *rect);

int addline(struct line *line);

int delline(struct line *line);

int addpoint(struct point *point);

int delpoint(struct point *point);

int addcircle(struct circle *circle);

int delcircle(struct circle *circle);

void drawgrid();

void delgrid();

int start_scene();

int redraw_scene();

int resize_window(int x, int y);
//int clear_scene();
#endif
