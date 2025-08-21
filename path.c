#include "shell.h"

char *find_command(char *command, char **envp)
{
    char *path_env = NULL;
    char *path_copy, *dir;
    char *fullpath;
    int i;

    for (i = 0; envp[i]; i++)
    {
        if (strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_env = envp[i] + 5;
            break;
        }
    }

    if (!path_env || strchr(command, '/'))
    {
        /* If command has '/' or PATH not set, return copy of command */
        fullpath = strdup(command);
        if (!fullpath)
        {
            perror("strdup");
            exit(1);
        }
        if (access(fullpath, X_OK) == 0)
            return fullpath;
        free(fullpath);
        return NULL;
    }

    path_copy = strdup(path_env);
    if (!path_copy)
    {
        perror("strdup");
        exit(1);
    }

    dir = strtok(path_copy, ":");
    while (dir)
    {
        fullpath = malloc(strlen(dir) + strlen(command) + 2);
        if (!fullpath)
        {
            perror("malloc");
            free(path_copy);
            exit(1);
        }
        sprintf(fullpath, "%s/%s", dir, command);

        if (access(fullpath, X_OK) == 0)
        {
            free(path_copy);
            return fullpath;
        }

        free(fullpath);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}
