#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include "input.h"
#include "scene.h"
#define TEST_GRID 1//
#ifdef TEST_GRID
#include "grid.h"
#endif


#define TEST_GRAPHICS 1

static struct icli_command *goal;
static struct icli_command *start;
static struct icli_command *block;
static struct icli_command *unblock;
static struct icli_command *clear;
static struct icli_command *getvals;
static struct icli_command *run;
static struct icli_command *load;
static struct icli_command *resize;
//static struct icli_command *exit_sim;

#ifdef TEST_GRAPHICS
static int goal_set = 0;
static int start_set = 0;
static struct circle start_circle;
static struct circle goal_circle;
#endif

static int check_number(int val,int range)
{
	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
	    || (errno != 0 && val == 0) || val > range || val < 1) {
		//perror("strtol");
		return EXIT_FAILURE;
	}
	return 0;
}

static enum icli_ret goal_cmd(char **argv, int argc, void *context)
{
	long x = 0;
	long y = 0;
	char *endptr;
	
	errno = 0;
	x = strtol(argv[0], &endptr, 10);
	if(check_number(x,grid_width) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range x operand\n");
		return ICLI_ERR;
	}
	
	endptr = NULL;
	errno = 0;
	y = strtol(argv[1], &endptr, 10);
	if(check_number(y, grid_height) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range y operand\n");
		return ICLI_ERR;
	}
#ifdef TEST_GRID
	if(put_goal(x,y)) {
		icli_err_printf("error placing goal\n");
		return ICLI_ERR;
	}
#else
#ifdef TEST_GRAPHICS
	//struct circle circle = GOAL_CIRCLE( 3 +((int)x*10), 3 + ((int)y*10), 5);
	struct circle circle = GOAL_CIRCLE( (int)x, (int)y, CIRCLE_RAD);
	if (goal_set)
		delcircle(&goal_circle);
	goal_set = 1;
	memcpy(&goal_circle, &circle, sizeof(struct circle));
	addcircle(&circle);
	redraw_scene();
#else
	icli_printf("goal not yet implemented\n");
#endif
#endif
		
	return ICLI_OK;
}

static struct icli_arg goal_args[] = {
	{.type = AT_None, .help = "x value"},
	{.type = AT_None, .help = "y value"},
};



static struct icli_command_params goal_params = {
	.parent     =   NULL,
	.name       =   "goal",
	.short_name =   "goal",
	.help       =   "set goal node",
	.command    =   goal_cmd,
	.argc       =   2,
	.argv       =   goal_args,
};



static enum icli_ret start_cmd(char **argv, int argc, void *context)
{
	long x = 0;
	long y = 0;
	char *endptr;
	
	errno = 0;
	x = strtol(argv[0], &endptr, 10);
	if (check_number(x, grid_width) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range x operand\n");
		return ICLI_ERR;
	}
	
	endptr = NULL;
	errno = 0;
	y = strtol(argv[1], &endptr, 10);
	if (check_number(y, grid_height) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range y operand\n");
		return ICLI_ERR;
	}
#ifdef TEST_GRID
	if(put_start(x,y)) {
		icli_err_printf("error placing start\n");
		return ICLI_ERR;
	}
#else
#ifdef TEST_GRAPHICS
	struct circle circle = START_CIRCLE( (int)x,(int)y, CIRCLE_RAD);
	if (start_set)
		delcircle(&start_circle);
	start_set = 1;
	memcpy(&start_circle, &circle, sizeof(struct circle));
	addcircle(&circle);
	redraw_scene();
#else
	icli_printf("start not yet implemented\n");
#endif
#endif
		
	return ICLI_OK;
}

static struct icli_arg start_args[] = {
	{.type = AT_None, .help = "x value"},
	{.type = AT_None, .help = "y value"},
};



static struct icli_command_params start_params = {
	.parent     =   NULL,
	.name       =   "start",
	.short_name =   "start",
	.help       =   "set start node",
	.command    =   start_cmd,
	.argc       =   2,
	.argv       =   start_args,
};


static enum icli_ret clear_cmd(char **argv, int argc, void *context)
{
	icli_printf("clearing board...\n");
	
	return ICLI_OK;
}

//static struct icli_arg clear_args[] = NULL;

static struct icli_command_params clear_params = {
	.parent     =   NULL,
	.name       =   "clear",
	.short_name =   "clear",
	.help       =   "clear entire board",
	.command    =   clear_cmd,
	.argc       =   0,
	.argv       =   NULL,
};

static enum icli_ret resize_cmd(char **argv, int argc, void *context)
{
	int x,y;
	char *endptr;
	
	errno = 0;
	x = strtol(argv[0], &endptr, 10);
	if (check_number(x, MAX_GRID_WIDTH) || *endptr != '\0') {
		icli_err_printf("x operand invalid value or larger than max of %d\n", MAX_GRID_WIDTH);
		return ICLI_ERR;
	}
	
	endptr = NULL;
	errno = 0;
	y = strtol(argv[1], &endptr, 10);
	if (check_number(y,MAX_GRID_HEIGHT) || *endptr != '\0') {
		icli_err_printf("y operand invalid value or larger than max of %d\n", MAX_GRID_HEIGHT);
		return ICLI_ERR;
	}
	icli_printf("resizing window...\n");
#ifdef TEST_GRID
	if(new_grid(x,y)) {
		icli_err_printf("error resizing grid\n");
		return ICLI_ERR;
	}
#else	
#ifdef TEST_GRAPHICS
	//these line will be moved into grid.c when it's ready
	resize_window(x,y);
	redraw_scene();
#else
	icli_printf("resize command not implemented yet\n");
#endif
#endif	
	return ICLI_OK;
}

static struct icli_arg resize_args[] = {
	{.type=AT_None, .help="x value"},
	{.type=AT_None, .help="y value"},
};


static struct icli_command_params resize_params = {
	.parent     =   NULL,
	.name       =   "resize",
	.short_name =   "resize",
	.help       =   "resize grid",
	.command    =   resize_cmd,
	.argc       =   2,
	.argv       =   resize_args,
};

static enum icli_ret block_cmd(char **argv, int argc, void *context)
{
	long x,y;
	char *endptr;
	
	errno=0;
	x = strtol(argv[0], &endptr, 10);
	if(check_number(x, grid_width - 1) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range x operand\n");
		return ICLI_ERR;
	}
	
	endptr = NULL;
	errno = 0;
	y = strtol(argv[1], &endptr,10);
	if(check_number(y, grid_height - 1) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range y operand\n");
		return ICLI_ERR;
	}
	
	icli_printf("adding block...\n");

#ifdef TEST_GRID
	if(set_tile(x,y,1)) {
		icli_err_printf("error blocking tile\n");
		return ICLI_ERR;
	}
#else	
#ifdef TEST_GRAPHICS
	//this is only for testing purposes, what goes in graph.c
	//MUST check for the presence of a block
	struct rect rect = BLOCK_RECT(x,y);
	addrect(&rect);
	redraw_scene();
#endif
#endif
	return ICLI_OK;
	
}

static struct icli_arg block_args[] = {
	{.type = AT_None, .help = "x value"},
	{.type = AT_None, .help = "y value"},
};

static struct icli_command_params block_params = {
	.parent     =   NULL,
	.name       =   "block",
	.short_name =   "block",
	.help       =   "block tile",
	.command    =   block_cmd,
	.argc       =   2,
	.argv       =   block_args,
};

static enum icli_ret unblock_cmd(char **argv, int argc, void *context)
{
	long x,y;
	char *endptr;
	
	errno = 0;
	x = strtol(argv[0], &endptr, 10);
	if(check_number(x, grid_width - 1) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range x operand\n");
		return ICLI_ERR;
	}
	
	endptr = NULL;
	errno = 0;
	y = strtol(argv[1], &endptr, 10);
	if(check_number(y, grid_height - 1) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range y operand\n");
		return ICLI_ERR;
	}
	
	icli_printf("removing block...\n");
#ifdef TEST_GRID
	if(set_tile(x,y,0)) {
		icli_err_printf("error unblocking tile\n");
		return ICLI_ERR;
	}
#else	
#ifdef TEST_GRAPHICS
	//this is only for testing purposes, what goes in graph.c
	//MUST check for the presence of a block
	struct rect rect = BLOCK_RECT(x,y);
	delrect(&rect);
	redraw_scene();
#endif
#endif
	
	return ICLI_OK;
	
}

static struct icli_arg unblock_args[] = {
	{.type = AT_None, .help = "x value"},
	{.type = AT_None, .help = "y value"},
};

static struct icli_command_params unblock_params = {
	.parent     =   NULL,
	.name       =   "unblock",
	.short_name =   "unblock",
	.help       =   "unblock tile",
	.command    =   unblock_cmd,
	.argc       =   2,
	.argv       =   unblock_args,
};

static enum icli_ret load_cmd(char **argv, int argc, void *context)
{
#ifdef TEST_GRID
	if(load_file(argv[1])) {
		icli_err_printf("error opening file\n");
		return ICLI_ERR;
	}
	#else
	icli_printf("command not yet implemented\n");
#endif
	return ICLI_OK;
}
static struct icli_arg load_args[] = {
	{.type = AT_None, .help = "name of file to load" },
};

static struct icli_command_params load_params = {
	.parent = NULL,
	.name = "load",
	.short_name = "load",
	.help = "load a test file onto the board",
	.command = load_cmd,
	.argc = 2,
	.argv = load_args,
};

static enum icli_ret run_cmd(char **argv, int argc, void *context)
{
#ifdef TEST_GRID
	if(run_algo(argv[0])) {
		icli_err_printf("error blocking tile\n");
		return ICLI_ERR;
	}
#else		
	icli_printf("command not yet implemented\n");
#endif
	return ICLI_OK;
}

static struct icli_arg_val run_arg_vals[] = {
	{.val = "astar", .help = "run the A\* algorithm"},
	{.val = "theta", .help = "run the theta\* algorithm"},
	{.val = NULL},
};

static struct icli_arg run_args[] = {
	{.type = AT_Val, .vals = run_arg_vals, .help = "name of algorithm to run:" },
	//{.type = AT_None, .help = "name of algorithm to run" },
};

static struct icli_command_params run_params = {
	.parent = NULL,
	.name = "run",
	.short_name = "run",
	.help = "run algorithm on current graph",
	.command = run_cmd,
	.argc = 1,
	.argv = run_args,
};

static enum icli_ret getvals_cmd(char **argv, int argc, void *context)
{
	long x = 0;
	long y = 0;
	char *endptr;
	double hval;
	double fval;
	errno = 0;
	x = strtol(argv[0], &endptr, 10);
	if(check_number(x,grid_width) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range x operand\n");
		return ICLI_ERR;
	}
	
	endptr = NULL;
	errno = 0;
	y = strtol(argv[1], &endptr, 10);
	if(check_number(y, grid_height) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range y operand\n");
		return ICLI_ERR;
	}
#ifdef TEST_GRID
	if(get_hval(x,y,&hval)) {
		icli_err_printf("error getting hval\n");
		return ICLI_ERR;
	}
	if(get_fval(x,y,&fval)) {
		icli_err_printf("error getting fval\n");
		return ICLI_ERR;
	}
	icli_printf("(%ld,%ld)\n\thval:%f.10\n\tfval:%f.10\n",x,y,hval,fval);
#else		
	icli_printf("command not yet implemented\n");
#endif
	return ICLI_OK;
}

static struct icli_arg getvals_args[] = {
	{.type = AT_None, .help = "x value"},
	{.type = AT_None, .help = "y value"},
};

static struct icli_command_params getvals_params = {
	.parent     =   NULL,
	.name       =   "getvals",
	.short_name =   "getvals",
	.help       =   "get h and f values of coordinate",
	.command    =   getvals_cmd,
	.argc       =   2,
	.argv       =   getvals_args,
};

struct command_list cmd_list[] = {
	{"goal",    &goal,    &goal_params,    (struct icli_arg **) &goal_args,    goal_cmd},
	{"start",   &start,   &start_params,   (struct icli_arg **) &start_args,   start_cmd},
	{"clear",   &clear,   &clear_params,   (struct icli_arg **) NULL,          clear_cmd},
	{"resize",  &resize,  &resize_params,  (struct icli_arg **) &resize_args,  resize_cmd},
	{"block",   &block,   &block_params,   (struct icli_arg **) &block_args,   block_cmd},
	{"unblock", &unblock, &unblock_params, (struct icli_arg **) &unblock_args, unblock_cmd},
	{"load",    &load,    &load_params,    (struct icli_arg **) &load_args,    load_cmd},
	{"run",     &run,     &run_params,     (struct icli_arg **) &run_args,     run_cmd},
	{"getvals", &getvals, &getvals_params, (struct icli_arg **) &getvals_args, getvals_cmd},
};



int pathfinder_register_commands()
{
        int ret;
	
        for (int i = 0; i < ARRAY_SIZE(cmd_list); i++) {
                ret = icli_register_command(cmd_list[i].cmd_params, cmd_list[i].cmd);
                if (ret) {
                        fprintf(stderr, "Unable to register command: %s\n", cmd_list[i].name);
                        ret = EXIT_FAILURE;
                        goto out;
                }
        }
        icli_commands_to_dot("path.dot");
out:
        return ret;
}
