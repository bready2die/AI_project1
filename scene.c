#include <stdlib.h>
#include <string.h>
#include "gfx.h"
#include "window.h"
#include "scene.h"

struct scene_context main_scene;

pthread_mutex_t scene_lock = PTHREAD_MUTEX_INITIALIZER;



LIST_HEAD(_scene_lines);
LIST_HEAD(_scene_points);
LIST_HEAD(_scene_circles);

int addline(struct line *line)
{
	struct line *newline = malloc(sizeof(*newline));
	memcpy(newline,line,sizeof( *line));
	pthread_mutex_lock(&scene_lock);
	list_add(&newline->list,&_scene_lines);
	pthread_mutex_unlock(&scene_lock);
	return 0;
}


int delline(struct line *line)
{
	struct list_head *i, *tmp;
	struct line *cur;
	pthread_mutex_lock(&scene_lock);
	list_for_each_safe(i,tmp,&_scene_lines) {
		cur = list_entry(i,struct line,list);
		if (!memcmp(line->bytes,cur->bytes,19)) {
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
	memcpy(newpoint,point,sizeof( *point));
	pthread_mutex_lock(&scene_lock);
	list_add(&newpoint->list,&_scene_points);
	pthread_mutex_unlock(&scene_lock);
	return 0;
}

int delpoint(struct point *point)
{
	struct list_head *i, *tmp;
	struct point *cur;
	pthread_mutex_lock(&scene_lock);
	list_for_each_safe(i,tmp,&_scene_points) {
		cur = list_entry(i,struct point,list);
		if (!memcmp(point->bytes,cur->bytes,11)) {
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
	newcircle->x=((PX_PER_SPACE*newcircle->x)+(CIRCLE_RAD/2));
	newcircle->y=((PX_PER_SPACE*newcircle->y)+(CIRCLE_RAD/2));
	pthread_mutex_lock(&scene_lock);
	list_add(&newcircle->list,&_scene_circles);
	pthread_mutex_unlock(&scene_lock);
	return 0;
}

/*
int addcircle(int x, int y, int rad, char r, char g, char b)
{
	struct circle circle = GOAL_CIRCLE( 3 +((int)x*10), 3 + ((int)y*10), 5);
	struct circle *newcircle = malloc(sizeof(*newcircle));
	memcpy(newcircle,circle,sizeof(*circle));
	pthread_mutex_lock(&scene_lock);
	list_add(&newcircle->list,&_scene_circles);
	pthread_mutex_unlock(&scene_lock);
	return 0;
}
*/
int delcircle(struct circle *circle)
{
	struct list_head *i, *tmp;
	struct circle *cur;
	struct circle *newcircle = malloc(sizeof(*newcircle));
	memcpy(newcircle,circle,sizeof(*circle));
	newcircle->x=((PX_PER_SPACE*newcircle->x)+(CIRCLE_RAD/2));
	newcircle->y=((PX_PER_SPACE*newcircle->y)+(CIRCLE_RAD/2));
	pthread_mutex_lock(&scene_lock);
	list_for_each_safe(i,tmp,&_scene_circles) {
		cur = list_entry(i,struct circle,list);
		if (!memcmp(newcircle->bytes,cur->bytes,15)) {
			list_del(i);
			free(cur);
			break;
		}
	}
	pthread_mutex_unlock(&scene_lock);
	free(newcircle);
	return 0;
}

void drawgrid()
{
	int width = WINWIDTH;
	int height = WINHEIGHT;
        for (int i = 5; i < height; i += 10) {
		struct line line = GRID_LINE(0,i,width,i);
                addline(&line);
        }
        for (int i = 5; i < width; i += 10) {
		struct line line = GRID_LINE(i,0,i,height);
                addline(&line);
        }
}

int redraw_scene()
{
		
	//printf("k\n");
	gfx_clear();
	gfx_flush();
	struct list_head *ipoint,*iline,*icircle;
	struct line *lcur;
	struct point *pcur;
	struct circle *ccur;
	pthread_mutex_lock(&scene_lock);
	list_for_each(ipoint,&_scene_points) {
		pcur = list_entry(ipoint, struct point,list);
		gfx_color(pcur->r,pcur->g,pcur->b);
		gfx_point(pcur->x,pcur->y);
	}
	list_for_each(iline,&_scene_lines) {
		//printf("oyoy\n");
		lcur = list_entry(iline,struct line,list);
		gfx_color(lcur->r,lcur->g,lcur->b);
		gfx_line(lcur->x1,lcur->y1,lcur->x2,lcur->y2);
	}
	list_for_each(icircle,&_scene_circles) {
		ccur = list_entry(icircle,struct circle, list);
		gfx_color(ccur->r,ccur->g,ccur->b);
		gfx_circle(ccur->x,ccur->y,ccur->rad);
	}
	gfx_flush();
	pthread_mutex_unlock(&scene_lock);
	return 0;
}

int clear_scene()
{
	
}

int start_scene()
{
        int ysize = WINHEIGHT;
        int xsize = WINWIDTH;
        // Open a new window for drawing.
        gfx_open(xsize,ysize,"Example Graphics Program");

        gfx_clear_color(BG_R,BG_G,BG_B);
        gfx_clear();
        // Set the current drawing color to green.
        gfx_color(255,0,0);
        //drawgrid();
        gfx_flush();
	
	drawgrid();

	return 0;
}
