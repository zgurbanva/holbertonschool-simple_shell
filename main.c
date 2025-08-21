#include "shell.h"

int main(int argc, char **argv, char **envp)
{
    char *line;
    size_t len;
    char *args[100];
    int line_no;
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
            printf("($) ");

        if (getline(&line, &len, stdin) == -1)
        {
            if (isatty(STDIN_FILENO))
                putchar('\n');
            break;
        }

        line[strcspn(line, "\n")] = 0;

        if (line[0] == '\0')
            continue;

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
            return status;
        }

        /* env built-in */
        if (strcmp(args[0], "env") == 0)
        {
            int j = 0;
            while (envp[j])
            {
                printf("%s\n", envp[j]);
                j++;
            }
            continue;
        }

        fullpath = find_command(args[0], envp);
        if (!fullpath)
        {
            fprintf(stderr, "%s: %d: %s: not found\n", argv[0], line_no, args[0]);
            status = 127;
            continue;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(fullpath);
            free(line);
            exit(1);
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
