#include "shell.h"

/* duplicate string (like strdup) */
char *strdup_custom(const char *s)
{
    size_t len;
    char *r;

    if (!s)
        return NULL;

    len = strlen(s);
    r = malloc(len + 1);
    if (!r)
        return NULL;

    memcpy(r, s, len + 1);
    return r;
}

/* get PATH from envp */
char *_getenv_from_envp(char **envp, const char *name)
{
    size_t i, len;

    if (!envp || !name)
        return NULL;

    len = strlen(name);
    for (i = 0; envp[i]; i++)
    {
        if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return envp[i] + len + 1;
    }
    return NULL;
}

/* join directory and file name into a malloc'd string */
char *join_path(const char *dir, const char *file)
{
    size_t ld, lf;
    char *res;

    if (!dir || !file)
        return NULL;

    ld = strlen(dir);
    lf = strlen(file);
    res = malloc(ld + 1 + lf + 1);
    if (!res)
        return NULL;

    memcpy(res, dir, ld);
    res[ld] = '/';
    memcpy(res + ld + 1, file, lf);
    res[ld + 1 + lf] = '\0';
    return res;
}

/* find command in PATH */
char *find_command(char *cmd, char **envp)
{
    char *path;
    char *pathdup;
    char *token;
    char *candidate = NULL;

    if (!cmd || !envp)
        return NULL;

    /* if command contains /, check directly */
    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return strdup_custom(cmd);
        return NULL;
    }

    path = _getenv_from_envp(envp, "PATH");
    if (!path)
        return NULL;

    pathdup = strdup_custom(path);
    if (!pathdup)
        return NULL;

    token = strtok(pathdup, ":");
    while (token)
    {
        candidate = join_path(token, cmd);
        if (candidate)
        {
            if (access(candidate, X_OK) == 0)
            {
                free(pathdup);
                return candidate;
            }
            free(candidate);
            candidate = NULL;
        }
        token = strtok(NULL, ":");
    }

    free(pathdup);
    return NULL;
}
