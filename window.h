#ifndef WINDOW_LIB
#define WINDOW_LIB 1

#include <pthread.h>
//#include "graphics.h"
//#include "scene.h"



struct window_context {
	int update_pending:1;
};
		
//pthread_t window_thread;

int start_window();

#endif
