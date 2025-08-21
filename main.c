#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/**
 * main - entry point of simple shell
 * @argc: argument count
 * @argv: argument vector
 * @envp: environment variables
 *
 * Return: exit status of last executed command
 */
int main(int argc, char **argv, char **envp)
{
    char *line = NULL;
    size_t len = 0;
    char *args[100]; /* max 100 arguments */
    int line_no = 0;
    int status = 0; /* last command exit status */

    (void)argc;

    while (1)
    {
        line_no++;
        if (isatty(STDIN_FILENO))
            printf("($) ");

        if (getline(&line, &len, stdin) == -1)
        {
            if (isatty(STDIN_FILENO))
                putchar('\n');
            break;
        }

        /* remove trailing newline */
        line[strcspn(line, "\n")] = 0;

        /* skip empty lines */
        if (line[0] == '\0')
            continue;

        /* tokenize input */
        int i = 0;
        char *token = strtok(line, " \t");
        while (token && i < 99)
        {
            args[i++] = token;
            token = strtok(NULL, " \t");
        }
        args[i] = NULL;

        if (!args[0])
            continue;

        /* handle built-in exit */
        if (strcmp(args[0], "exit") == 0)
        {
            free(line);
            return status; /* return last command exit status */
        }

        /* find full path of command */
        char *fullpath = find_command(args[0], envp);
        if (!fullpath)
        {
            fprintf(stderr, "%s: %d: %s: not found\n", argv[0], line_no, args[0]);
            status = 127; /* command not found */
            continue;
        }

        /* execute command */
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(fullpath);
            free(line);
            exit(1);
        }

        if (pid == 0) /* child */
        {
            execve(fullpath, args, envp);
            perror("execve");
            exit(1);
        }
        else /* parent */
        {
            int wstatus;
            waitpid(pid, &wstatus, 0);
            if (WIFEXITED(wstatus))
                status = WEXITSTATUS(wstatus);
            else
                status = 1;
        }

        free(fullpath);
    }

    free(line);
    return status;
}
