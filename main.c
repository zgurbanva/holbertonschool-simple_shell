#include "shell.h"

int main(int argc, char **argv, char **envp)
{
    /* All variable declarations at the top */
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    unsigned int line_no = 0;
    char *token;
    char *args[100];

    int i;
    char *fullpath;
    pid_t pid;
    int status;

    (void)argc;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "($) ", 4);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
            break;

        line_no++;
        if (nread > 0 && line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* Tokenize input */
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

        /* Handle exit builtin */
        if (strcmp(args[0], "exit") == 0)
        {
            free(line);
            return 0;
        }

        /* Find command in PATH */
        fullpath = find_command(args[0], envp);
        if (!fullpath)
        {
            fprintf(stderr, "%s: %u: %s: not found\n", argv[0], line_no, args[0]);
            continue;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(fullpath);
            continue;
        }

        if (pid == 0)
        {
            /* child */
            execve(fullpath, args, envp);
            fprintf(stderr, "%s: %u: %s: not found\n", argv[0], line_no, args[0]);
            _exit(127);
        }
        else
        {
            waitpid(pid, &status, 0);
            free(fullpath);
        }
    }

    free(line);
    return 0;
}
