#include <stdio.h>
#include "gfx.h"
#include "scene.h"
#include "window.h"

pthread_t window_thread;

static struct window_context wcontext;

void main_loop()
{
	//char c;
	while (1) {		
		redraw_scene();
		
		gfx_wait();
	}
}


void *window_thread_fun(void *arg)
{
	start_scene();
	//start_graphics();
	main_loop();
	return NULL;
}



int start_window()
{
	if (pthread_create(&window_thread,NULL,window_thread_fun,&wcontext)) {
		printf("failed to create window thread\n");
		return -1;
	}
	return 0;
}
