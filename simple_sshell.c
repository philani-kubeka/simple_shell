#include "sshell.h"

void exec_command(char *cmd);
void handle_exit(void);
void handle_env(void);

/**
 * run_shell - unix command line interpreter.
 *
 * Return: 1 or 0.
 */
void run_shell(void)
{
	char *input = NULL;
	size_t input_size = 0;

	while (1)
	{
		printf(prompt);
		if (getline(&input, &input_size, stdin) == -1)
		{
			printf("\n");
			free(input);
			break;
		}

		input[strcmp(input, "\n")] = '\0';

		if (strcmp(input, "exit") == 0)
		{
			handle_exit();
		}
		else if (strcmp(input, "env") == 0)
		{
			handle_env();
		}
		else
		{
			exec_command(input);
		}
	}
	free(input);
}

/**
 * exec_command - executes the give command.
 *
 * @command: command given.
 *
 * Return: always 0.
 */
void exec_command(char *command)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execve(command, (char *const *)&command, environ) == -1)
		{
			perror(command);
			_exit(EXIT_FAILURE);
		}
	}
	else
	{
		int status;

		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
		{
			int exit_status;

			exit_status = WEXITSTATUS(status);
			if (exit_status != 0)
			{
				fprintf(stderr, "./simple_shell: %s: exit status: %d\n",
						command, exit_status);
			}
		}
		else if (WIFSIGNALED(status))
		{
			int signal_number;

			signal_number = WTERMSIG(status);
			fprintf(stderr, "./simple_shell: %s: Terminated by signal: %d\n",
					command, signal_number);
		}
	}
}

/**
 * handle_exit - exits the shell.
 *
 * Return: always 0.
 */
void handle_exit(void)
{
	exit(0);
}

/**
 * handle_env - handles the env.
 *
 * Return: always 0.
 */
void handle_env(void)
{
	int i;

	for (i = 0; environ[i] != NULL; i++)
	{
		printf("%s\n", environ[i]);
	}
}
