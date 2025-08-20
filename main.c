#include "shell.h"

/**
 * print_prompt - Prints the interactive prompt.
 */
void print_prompt(void)
{
	write(STDOUT_FILENO, PROMPT_STR, sizeof(PROMPT_STR) - 1);
}

/**
 * first_token - Returns first whitespace-delimited token from a line.
 * @line: Mutable input buffer (modified by strtok).
 *
 * Return: Pointer to first token, or NULL if none.
 */
char *first_token(char *line)
{
	return (strtok(line, DELIMS));
}

/**
 * run_command - Forks and runs a single-word command with execve.
 * @cmd: Command path or name (no args supported).
 * @pname: Program name (argv[0]) for perror prefix.
 *
 * Return: 0 on completion (child exit status not propagated).
 */
int run_command(const char *cmd, const char *pname)
{
	pid_t pid;
	int status = 0;
	char *av[2];

	if (!cmd || !*cmd)
		return (0);

	av[0] = (char *)cmd;
	av[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror((char *)pname);
		return (0);
	}
	if (pid == 0)
	{
		if (execve(cmd, av, environ) == -1)
		{
			perror((char *)pname);
			_exit(127);
		}
	}
	if (waitpid(pid, &status, 0) == -1)
		perror((char *)pname);

	return (0);
}

/**
 * shell_loop - Main read/execute loop.
 * @pname: Program name for error messages (argv[0]).
 *
 * Return: 0 on normal exit.
 */
int shell_loop(const char *pname)
{
	char *line = NULL, *cmd = NULL;
	size_t len = 0;
	ssize_t nread;
	int interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
			print_prompt();

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		cmd = first_token(line);
		if (cmd == NULL)
			continue;

		run_command(cmd, pname);
	}

	free(line);
	return (0);
}

/**
 * main - Entry point.
 * @argc: Argument count.
 * @argv: Argument vector.
 *
 * Return: Exit status.
 */
int main(int argc, char **argv)
{
	(void)argc;
	return (shell_loop(argv[0]));
}
