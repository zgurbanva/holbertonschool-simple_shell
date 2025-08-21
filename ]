#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv, char **envp)
{
    char *line;
    size_t len;
    ssize_t nread;
    char *args[100];
    unsigned int line_no;
    int status;
    int i;
    char *token;
    char *fullpath;
    pid_t pid;
    int wstatus;

    (void)argc;

    line = NULL;
    len = 0;
    line_no = 0;
    status = 0;

    while (1)
    {
        line_no++;

        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "($) ", 4);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
            break;

        if (nread > 0 && line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* tokenize input */
        i = 0;
        token = strtok(line, " \t");
        while (token && i < 99)
        {
            args[i++] = token;
            token = strtok(NULL, " \t");
        }
        args[i] = NULL;

        if (!args[0])
            continue;

        /* exit built-in */
        if (strcmp(args[0], "exit") == 0)
        {
            free(line);
            return 0;
        }

        /* find command in PATH */
        fullpath = find_command(args[0], envp);
        if (!fullpath)
        {
            fprintf(stderr, "%s: %d: %s: not found\n", argv[0], line_no, args[0]);
            status = 127;
            continue; /* do NOT fork */
        }

        /* fork and execute */
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(fullpath);
            continue;
        }

        if (pid == 0)
        {
            execve(fullpath, args, envp);
            perror("execve");
            exit(1);
        }
        else
        {
            waitpid(pid, &wstatus, 0);
        }

        free(fullpath);
    }

    free(line);
    return status;
}

