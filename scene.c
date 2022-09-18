#include <stdlib.h>
#include <string.h>
#include "gfx.h"
#include "window.h"
#include "scene.h"
//both grid_width and grid_height should be moved into graph.c when it is ready for testing

int grid_width=DEF_GRID_WIDTH;

int grid_height=DEF_GRID_HEIGHT;

struct scene_context main_scene;

pthread_mutex_t scene_lock = PTHREAD_MUTEX_INITIALIZER;



LIST_HEAD(_scene_lines);
LIST_HEAD(_scene_points);
LIST_HEAD(_scene_circles);
LIST_HEAD(_scene_rects);

int addrect(struct rect *rect)
{
	struct rect *newrect = malloc(sizeof(*newrect));
	
	memcpy(newrect,rect,sizeof(*rect));
	newrect->x = (WIN_BORDER + (PX_PER_SPACE * newrect->x));
	newrect->y = (WIN_BORDER + (PX_PER_SPACE * newrect->y));
	newrect->h = (PX_PER_SPACE * newrect->h);
	newrect->w = (PX_PER_SPACE * newrect->w);

	pthread_mutex_lock(&scene_lock);
	list_add(&newrect->list,&_scene_rects);
	pthread_mutex_unlock(&scene_lock);
	
	return 0;
}

int delrect(struct rect *rect)
{
	struct list_head *i,*tmp;
	struct rect *cur;
	struct rect newrect;
	
	memcpy(&newrect,rect,sizeof(*rect));
	newrect.x = (WIN_BORDER + (PX_PER_SPACE * newrect.x));
	newrect.y = (WIN_BORDER + (PX_PER_SPACE * newrect.y));
	newrect.h = (PX_PER_SPACE * newrect.h);
	newrect.w = (PX_PER_SPACE * newrect.w);

	pthread_mutex_lock(&scene_lock);
	list_for_each_safe(i,tmp,&_scene_rects) {
		cur = list_entry(i,struct rect, list);
		if(!memcmp(newrect.bytes,cur->bytes,sizeof(_RECT_DATA))) {
			list_del(i);
			free(cur);
			break;
		}
	}
	pthread_mutex_unlock(&scene_lock);
	
	return 0;
}

int addline(struct line *line)
{
	struct line *newline = malloc(sizeof(*newline));
	
	memcpy(newline,line,sizeof( *line));
	newline->x1 = (WIN_BORDER + (PX_PER_SPACE * newline->x1));
	newline->y1 = (WIN_BORDER + (PX_PER_SPACE * newline->y1));
	newline->x2 = (WIN_BORDER + (PX_PER_SPACE * newline->x2));
	newline->y2 = (WIN_BORDER + (PX_PER_SPACE * newline->y2));

	pthread_mutex_lock(&scene_lock);
	list_add(&newline->list,&_scene_lines);
	pthread_mutex_unlock(&scene_lock);

	return 0;
}


int delline(struct line *line)
{
	struct list_head *i, *tmp;
	struct line *cur;
	struct line newline;

	memcpy(&newline,line,sizeof( *line));
	newline.x1 = (WIN_BORDER + (PX_PER_SPACE * newline.x1));
	newline.y1 = (WIN_BORDER + (PX_PER_SPACE * newline.y1));
	newline.x2 = (WIN_BORDER + (PX_PER_SPACE * newline.x2));
	newline.y2 = (WIN_BORDER + (PX_PER_SPACE * newline.y2));

	pthread_mutex_lock(&scene_lock);
	list_for_each_safe(i,tmp,&_scene_lines) {
		cur = list_entry(i,struct line,list);
		if (!memcmp(newline.bytes,cur->bytes,sizeof(_LINE_DATA))) {
			list_del(i);
			free(cur);
			break;
		}
	}
	pthread_mutex_unlock(&scene_lock);
	
	return 0;
}


int addpoint(struct point *point)
{
	struct point *newpoint = malloc(sizeof(*newpoint));

	memcpy(newpoint,point,sizeof(*point));
	newpoint->x = (WIN_BORDER + (PX_PER_SPACE * newpoint->x));
	newpoint->y = (WIN_BORDER + (PX_PER_SPACE * newpoint->y));
	
	pthread_mutex_lock(&scene_lock);
	list_add(&newpoint->list,&_scene_points);
	pthread_mutex_unlock(&scene_lock);

	return 0;
}

int delpoint(struct point *point)
{
	struct list_head *i, *tmp;
	struct point *cur;
	struct point newpoint;

	memcpy(&newpoint,point,sizeof(*point));
	newpoint.x = (WIN_BORDER + (PX_PER_SPACE * newpoint.x));
	newpoint.y = (WIN_BORDER + (PX_PER_SPACE * newpoint.y));

	pthread_mutex_lock(&scene_lock);
	list_for_each_safe(i,tmp,&_scene_points) {
		cur = list_entry(i,struct point,list);
		if (!memcmp(newpoint.bytes,cur->bytes,sizeof(_POINT_DATA))) {
			list_del(i);
			free(cur);
			break;
		}
	}
	pthread_mutex_unlock(&scene_lock);
	
	return 0;
}

int addcircle(struct circle *circle)
{
	struct circle *newcircle = malloc(sizeof(*newcircle));

	memcpy(newcircle,circle,sizeof(*circle));
	newcircle->x = (WIN_BORDER + (PX_PER_SPACE * newcircle->x) - (newcircle->rad));
	newcircle->y = (WIN_BORDER + (PX_PER_SPACE * newcircle->y) - (newcircle->rad));

	pthread_mutex_lock(&scene_lock);
	list_add(&newcircle->list,&_scene_circles);
	pthread_mutex_unlock(&scene_lock);
	
	return 0;
}


int delcircle(struct circle *circle)
{
	struct list_head *i, *tmp;
	struct circle *cur;
	struct circle newcircle;

	memcpy(&newcircle,circle,sizeof(*circle));
	newcircle.x = (WIN_BORDER + (PX_PER_SPACE * newcircle.x) - (newcircle.rad));
	newcircle.y = (WIN_BORDER + (PX_PER_SPACE * newcircle.y) - (newcircle.rad));

	pthread_mutex_lock(&scene_lock);
	list_for_each_safe(i,tmp,&_scene_circles) {
		cur = list_entry(i,struct circle,list);
		if (!memcmp(newcircle.bytes,cur->bytes,sizeof(_CIRCLE_DATA))) {
			list_del(i);
			free(cur);
			break;
		}
	}
	pthread_mutex_unlock(&scene_lock);
	
	return 0;
}

void drawgrid()
{
	for (int i = 0; i < grid_height + 1; i++) {
		struct line line = GRID_LINE(0,i,grid_width,i);
                addline(&line);
        }
        for (int i = 0; i < grid_width + 1; i++) {
		struct line line = GRID_LINE(i,0,i,grid_height);
                addline(&line);
        }
}

void delgrid()
{
	for (int i = 0; i < grid_height + 1; i++) {
		struct line line = GRID_LINE(0,i,grid_width,i);
                delline(&line);
        }
        for (int i = 0; i < grid_width + 1; i++) {
		struct line line = GRID_LINE(i,0,i,grid_height);
                delline(&line);
        }
}

int redraw_scene()
{
	struct list_head *ipoint,*iline,*icircle,*irect;
	struct line *lcur;
	struct point *pcur;
	struct circle *ccur;
	struct rect *rcur;

	gfx_clear();

	pthread_mutex_lock(&scene_lock);
	list_for_each(irect,&_scene_rects) {
		rcur = list_entry(irect, struct rect,list);
		gfx_color(rcur->r,rcur->g,rcur->b);
		gfx_rect(rcur->x,rcur->y,rcur->w,rcur->h);
	}
	list_for_each(ipoint,&_scene_points) {
		pcur = list_entry(ipoint, struct point,list);
		gfx_color(pcur->r,pcur->g,pcur->b);
		gfx_point(pcur->x,pcur->y);
	}
	list_for_each(iline,&_scene_lines) {
		lcur = list_entry(iline,struct line,list);
		gfx_color(lcur->r,lcur->g,lcur->b);
		gfx_line(lcur->x1,lcur->y1,lcur->x2,lcur->y2);
	}
	list_for_each(icircle,&_scene_circles) {
		ccur = list_entry(icircle,struct circle, list);
		gfx_color(ccur->r,ccur->g,ccur->b);
		gfx_circle(ccur->x,ccur->y,ccur->rad);
	}
	pthread_mutex_unlock(&scene_lock);
	
	gfx_flush();
	return 0;
}

int resize_window(int x, int y)
{
	
	gfx_resize_window((2 * WIN_BORDER) + (PX_PER_SPACE * x),
			  (2 * WIN_BORDER) + (PX_PER_SPACE * y));
	delgrid();
	grid_width = x;
	grid_height = y;
	drawgrid();
	
	return 0;
}

int start_scene()
{
        // Open a new window for drawing.
        gfx_open(WINWIDTH,WINHEIGHT,"Pathfinder");

        gfx_clear_color(BG_R,BG_G,BG_B);
        gfx_clear();
        // Set the current drawing color to green.
        //gfx_color(255,0,0);
        //drawgrid();
        gfx_flush();
	
	drawgrid();

	return 0;
}
