#ifndef INPUT_LIB
#define INPUT_LIB 1

#include "icli.h"
//#include "graphics.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


struct command_list
{
	char *name;
	struct icli_command **cmd;
	struct icli_command_params *cmd_params;
	struct icli_arg **cmd_args;
	enum icli_ret (*cmd_fn)(char **argv, int argc, void *context);
};

int pathfinder_register_commands(void);

#endif
