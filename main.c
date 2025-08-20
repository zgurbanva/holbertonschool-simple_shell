#include "shell.h"

/**
 * read_line - Reads one line of input from stdin.
 *
 * Return: Pointer to line buffer (must be freed), or NULL on EOF/error.
 */
char *read_line(void)
{
	char *line = NULL;
	size_t len = 0;

	if (getline(&line, &len, stdin) == -1)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

/**
 * parse_line - Tokenizes a line into arguments.
 * @line: Input string.
 *
 * Return: Array of strings (NULL-terminated). Must be freed by caller.
 */
char **parse_line(char *line)
{
	size_t bufsize = TOK_BUFSIZE, idx = 0;
	char **tokens = malloc(sizeof(char *) * bufsize);
	char *tok;

	if (!tokens)
		return (NULL);

	tok = strtok(line, TOK_DELIM);
	while (tok)
	{
		tokens[idx++] = tok;
		if (idx + 1 >= bufsize)
		{
			bufsize *= 2;
			tokens = realloc(tokens, sizeof(char *) * bufsize);
			if (!tokens)
				return (NULL);
		}
		tok = strtok(NULL, TOK_DELIM);
	}
	tokens[idx] = NULL;
	return (tokens);
}

/**
 * execute - Forks and executes a command.
 * @args: Argument vector.
 * @pname: Program name for errors.
 *
 * Return: Status code.
 */
int execute(char **args, char *pname)
{
	pid_t pid;
	int status;

	if (!args[0])
		return (0);

	if (strcmp(args[0], "exit") == 0)
		exit(0);
	if (strcmp(args[0], "env") == 0)
	{
		size_t i;

		for (i = 0; environ[i]; i++)
			printf("%s\n", environ[i]);
		return (0);
	}

	pid = fork();
	if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			fprintf(stderr, "%s: 1: %s: not found\n", pname, args[0]);
			exit(127);
		}
	}
	else if (pid < 0)
		perror("fork");
	else
		waitpid(pid, &status, 0);

	return (0);
}

/**
 * loop - Main shell loop.
 * @pname: Program name.
 */
void loop(char *pname)
{
	char *line;
	char **args;
	int interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
			printf("($) ");
		line = read_line();
		if (!line)
		{
			if (interactive)
				printf("\n");
			break;
		}
		args = parse_line(line);
		execute(args, pname);
		free(args);
		free(line);
	}
}

/**
 * main - Entry point.
 * @argc: Arg count.
 * @argv: Arg vector.
 *
 * Return: 0
 */
int main(int argc, char **argv)
{
	(void)argc;
	loop(argv[0]);
	return (0);
}
