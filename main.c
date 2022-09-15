#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <pthread.h>
#include "icli.h"
#include "input.h"
#include "window.h"
//#include "graphics.h"

struct my_context {
		int something;
		FILE *log;
};

struct my_context user_context;

static void cli_cmd_hook(const char *cmd, char *argv[], int argc, void *context)
{
		struct my_context *self = context;
		fprintf(self->log, "CMD: %s", cmd);
		
		for (int i = 0; i < argc; ++i) {
				fprintf(self->log, " %s", argv[i]);
		}
		fprintf(self->log, "\n");
}

static void cli_out_hook(const char *format, va_list args, void *context)
{
		struct my_context *self = context;
		
		vfprintf(self->log, format, args);
}

static void cli_err_hook(const char *format, va_list args, void *context)
{
		struct my_context *self = context;
		fprintf(self->log, "ERR:");
		
		vfprintf(self->log, format, args);
}


struct icli_params params = {
		.user_data = &user_context,
		.history_size = 10,
		.app_name = "pathfinder",
		.prompt = "path_con",
		.hist_file = "./icli_history",
		.cmd_hook = cli_cmd_hook,
		.out_hook = cli_out_hook,
		.err_hook = cli_err_hook,
};


/*
int pathfinder_register_commands()
{
		int ret;
		ret=icli_register_command(&goal_params, &goal);
		if (ret) {
				fprintf(stderr, "Unable to register command: %s\n", goal_params.name);
				ret = EXIT_FAILURE;
				goto out;
		}
		ret=icli_register_command(&start_params, &start);
		if (ret) {
				fprintf(stderr, "Unable to register command: %s\n", start_params.name);
				ret = EXIT_FAILURE;
				goto out;
		}
		icli_commands_to_dot("path.dot");
out:
		return ret;
}
*/
/*
int pathfinder_register_commands()
{
		int ret;
		for(int i = 0; i < ARRAY_SIZE(cmd_list); i++) {
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
*/

int main(int argc, char **argv)
{
		int res;
		int ret = EXIT_SUCCESS;
		user_context = (struct my_context){.log = fopen("./cli.log", "w")};

		if (NULL == user_context.log) {
				fprintf(stderr, "Unable to open log file:%m\n");
				return EXIT_FAILURE;
		}
		
		res = icli_init(&params);
		if (res) {
				fprintf(stderr, "Unable to init icli\n");
				icli_cleanup();
				return EXIT_FAILURE;
		}
		if (pathfinder_register_commands()) {
				printf("error encountered. exiting...\n");
				goto out;
		}
		printf("everything went smoothly\n");
		//start_graphics();
		start_window();
		icli_run();
out:
		fclose(user_context.log);
		icli_cleanup();
		return ret;
		
}
