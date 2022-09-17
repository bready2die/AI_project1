#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include "input.h"
#include "scene.h"

//extern int grid_width;
//extern int grid_height;

#define TEST_CIRCLES 1

static struct icli_command *goal;
static struct icli_command *start;
static struct icli_command *block;
static struct icli_command *unblock;
static struct icli_command *clear;
static struct icli_command *clearblock;
static struct icli_command *weight;
static struct icli_command *run;
static struct icli_command *runfile;
static struct icli_command *resize;
//static struct icli_command *exit_sim;

#ifdef TEST_CIRCLES
static int goal_set = 0;

static struct circle goal_circle;
#endif

static int check_number(int val,int range)
{
	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
	    || (errno != 0 && val == 0) || val > range || val < 0) {
		perror("strtol");
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
	x = strtol(argv[0],&endptr,10);
	if(check_number(x,grid_width) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range x operand\n");
		return ICLI_ERR;
	}
	endptr = NULL;
	errno = 0;
	y = strtol(argv[1],&endptr,10);
	if(check_number(y,grid_height) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range y operand\n");
		return ICLI_ERR;
	}
#ifdef TEST_CIRCLES
	//struct circle circle = GOAL_CIRCLE( 3 +((int)x*10), 3 + ((int)y*10), 5);
	struct circle circle = GOAL_CIRCLE( (int)x,(int)y, CIRCLE_RAD);
	if (goal_set)
		delcircle(&goal_circle);
	goal_set = 1;
	memcpy(&goal_circle,&circle,sizeof(struct circle));
	addcircle(&circle);
	redraw_scene();
#endif
	icli_printf("goal not yet implemented\n");
	errno = 0;
		
	return ICLI_OK;
}

static struct icli_arg goal_args[] = {
	{.type=AT_None, .help="x value"},
	{.type=AT_None, .help="y value"},
};



static struct icli_command_params goal_params = {
	.parent = NULL,
	.name = "goal",
	.short_name = "goal",
	.help = "set goal node",
	.command = goal_cmd,
	.argc = 2,
	.argv = goal_args,
};



static enum icli_ret start_cmd(char **argv, int argc, void *context)
{
	long x = 0;
	long y = 0;
	char *endptr;
	errno = 0;
	x = strtol(argv[0],&endptr,10);
	if (check_number(x,grid_width) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range x operand\n");
		return ICLI_ERR;
	}
	endptr = NULL;
	errno = 0;
	y = strtol(argv[1],&endptr,10);
	if (check_number(y,grid_height) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range y operand\n");
		return ICLI_ERR;
	}
	icli_printf("start not yet implemented\n");
	errno=0;
		
	return ICLI_OK;
}

static struct icli_arg start_args[] = {
	{.type=AT_None, .help="x value"},
	{.type=AT_None, .help="y value"},
};



static struct icli_command_params start_params = {
	.parent = NULL,
	.name = "start",
	.short_name = "start",
	.help = "set start node",
	.command = start_cmd,
	.argc = 2,
	.argv = start_args,
};


static enum icli_ret clear_cmd(char **argv, int argc, void *context)
{
	icli_printf("clearing board...\n");
	
	return ICLI_OK;
}

//static struct icli_arg clear_args[] = NULL;

static struct icli_command_params clear_params = {
	.parent = NULL,
	.name = "clear",
	.short_name = "clear",
	.help = "clear entire board",
	.command = clear_cmd,
	.argc = 0,
	.argv = NULL,
};

static enum icli_ret resize_cmd(char **argv, int argc, void *context)
{
	int x,y;
	char *endptr;
	errno = 0;
	x = strtol(argv[0],&endptr,10);
	if (check_number(x,MAX_GRID_WIDTH) || *endptr != '\0') {
		icli_err_printf("x operand invalid value or larger than max of %d\n",MAX_GRID_WIDTH);
		return ICLI_ERR;
	}
	endptr = NULL;
	errno = 0;
	y = strtol(argv[1],&endptr,10);
	if (check_number(y,MAX_GRID_HEIGHT) || *endptr != '\0') {
		icli_err_printf("y operand invalid value or larger than max of %d\n",MAX_GRID_HEIGHT);
		return ICLI_ERR;
	}
        icli_printf("resizing window...\n");
	//this line will be moved into grid.c when it's ready
	resize_window(x,y);
	redraw_scene();
		
	return ICLI_OK;
}

static struct icli_arg resize_args[] = {
	{.type=AT_None, .help="x value"},
	{.type=AT_None, .help="y value"},
};


static struct icli_command_params resize_params = {
	.parent = NULL,
	.name = "resize",
	.short_name = "resize",
	.help = "resize grid",
	.command = resize_cmd,
	.argc = 2,
	.argv = resize_args,
};

static enum icli_ret block_cmd(char **argv, int argc, void *context)
{
	int x,y;
	char *endptr;
	x = strtol(argv[0],&endptr,10);
	if(check_number(x,grid_width-1) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range x operand\n");
		return ICLI_ERR;
	}
	endptr = NULL;
	errno = 0;
	y = strtol(argv[1],&endptr,10);
	if(check_number(y,grid_height-1) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range y operand\n");
		return ICLI_ERR;
	}
	icli_printf("adding block...\n");
	//this is only for testing purposes, what goes in graph.c
	//MUST check for the presence of a block
	struct rect rect = BLOCK_RECT(x,y);
	addrect(&rect);
	redraw_scene();
	return ICLI_OK;
	
}

static struct icli_arg block_args[] = {
	{.type = AT_None, .help = "x value"},
	{.type = AT_None, .help = "y value"},
};

static struct icli_command_params block_params = {
	.parent = NULL,
	.name = "block",
	.short_name = "block",
	.help = "block tile",
	.command = block_cmd,
	.argc = 2,
	.argv = block_args,
};

static enum icli_ret unblock_cmd(char **argv, int argc, void *context)
{
	int x,y;
	char *endptr;
	x = strtol(argv[0],&endptr,10);
	if(check_number(x,grid_width-1) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range x operand\n");
		return ICLI_ERR;
	}
	endptr = NULL;
	errno = 0;
	y = strtol(argv[1],&endptr,10);
	if(check_number(y,grid_height-1) || *endptr != '\0') {
		icli_err_printf("Invalid or out of range y operand\n");
		return ICLI_ERR;
	}
	icli_printf("removing block...\n");
	//this is only for testing purposes, what goes in graph.c
	//MUST check for the presence of a block
	struct rect rect = BLOCK_RECT(x,y);
	delrect(&rect);
	redraw_scene();
	return ICLI_OK;
	
}

static struct icli_arg unblock_args[] = {
	{.type = AT_None, .help = "x value"},
	{.type = AT_None, .help = "y value"},
};

static struct icli_command_params unblock_params = {
	.parent = NULL,
	.name = "unblock",
	.short_name = "unblock",
	.help = "unblock tile",
	.command = unblock_cmd,
	.argc = 2,
	.argv = unblock_args,
};

struct command_list cmd_list[] = {
	{"goal", &goal,&goal_params ,(struct icli_arg **)&goal_args, goal_cmd},
	{"start", &start, &start_params, (struct icli_arg **)&start_args, start_cmd},
	{"clear", &clear,&clear_params,(struct icli_arg **)NULL,clear_cmd},
	{"resize",&resize,&resize_params,(struct icli_arg **)&resize_args,resize_cmd},
	{"block",&block,&block_params,(struct icli_arg **)&block_args,block_cmd},
	{"unblock",&unblock,&unblock_params,(struct icli_arg **)&unblock_args,unblock_cmd},
};



int pathfinder_register_commands()
{
        int ret;
        for (int i = 0; i < ARRAY_SIZE(cmd_list); i++) {
                ret = icli_register_command(cmd_list[i].cmd_params,cmd_list[i].cmd);
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
