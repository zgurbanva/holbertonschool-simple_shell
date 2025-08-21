#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *find_command(char *cmd, char **envp)
{
    char *path_env;
    char *path_copy;
    char *token;
    char *fullpath;
    size_t len;

    if (!cmd)
        return NULL;

    if (strchr(cmd, '/')) /* command contains / → direct path */
    {
        if (access(cmd, X_OK) == 0)
            return strdup(cmd);
        else
            return NULL;
    }

    /* get PATH */
    path_env = NULL;
    while (*envp)
    {
        if (strncmp(*envp, "PATH=", 5) == 0)
        {
            path_env = *envp + 5;
            break;
        }
        envp++;
    }

    if (!path_env || !*path_env)
        return NULL; /* PATH empty or unset */

    path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;

    token = strtok(path_copy, ":");
    while (token)
    {
        len = strlen(token) + 1 + strlen(cmd) + 1;
        fullpath = malloc(len);
        if (!fullpath)
        {
            free(path_copy);
            return NULL;
        }
        strcpy(fullpath, token);
        strcat(fullpath, "/");
        strcat(fullpath, cmd);

        if (access(fullpath, X_OK) == 0)
        {
            free(path_copy);
            return fullpath;
        }

        free(fullpath);
        token = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

