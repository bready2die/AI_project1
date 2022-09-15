#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include "input.h"
#include "scene.h"

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
static struct icli_command *exit_sim;

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
		if(check_number(x,100) || *endptr != '\0') {
				icli_err_printf("Invalid or out of range x operand\n");
				return ICLI_ERR;
		}
		endptr = NULL;
		errno = 0;
		y = strtol(argv[1],&endptr,10);
		if(check_number(y,50) || *endptr != '\0') {
				icli_err_printf("Invalid or out of range y operand\n");
				return ICLI_ERR;
		}
#ifdef TEST_CIRCLES
		struct circle circle = {
				.x = 3 + ((int)x*10),
				.y = 3 + ((int)y*10),
				.rad = 5,
				.r = 0,
				.g = 255,
				.b = 0,
		};
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
		if (check_number(x,100) || *endptr != '\0') {
				icli_err_printf("Invalid or out of range x operand\n");
				return ICLI_ERR;
		}
		endptr = NULL;
		errno = 0;
		y = strtol(argv[1],&endptr,10);
		if (check_number(y,50) || *endptr != '\0') {
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


struct command_list cmd_list[] = {
		{"goal", &goal,&goal_params ,&goal_args, goal_cmd},
		{"start", &start, &start_params, &start_args, start_cmd},
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
