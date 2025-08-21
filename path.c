/* path.c */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

/* get PATH value from envp (returns pointer into envp string, not malloc'd) */
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

/* join directory and filename into a newly malloc'd string: "dir/file" */
char *join_path(const char *dir, const char *file)
{
    size_t ld, lf;
    char *res;

    if (!dir || !file)
        return NULL;
    ld = strlen(dir);
    lf = strlen(file);
    res = malloc(ld + 1 + lf + 1); /* dir + '/' + file + '\0' */
    if (!res)
        return NULL;
    memcpy(res, dir, ld);
    res[ld] = '/';
    memcpy(res + ld + 1, file, lf);
    res[ld + 1 + lf] = '\0';
    return res;
}

/* make a malloc'd copy of s */
char *strdup_custom(const char *s)
{
    size_t l;
    char *r;

    if (!s)
        return NULL;
    l = strlen(s);
    r = malloc(l + 1);
    if (!r) return NULL;
    memcpy(r, s, l + 1);
    return r;
}

/* find command in PATH; returns malloc'd path if found, else NULL.
   If cmd contains a '/' check it directly (absolute or relative). */
char *find_command(char *cmd, char **envp)
{
    char *path, *pathdup, *token, *candidate = NULL;
    char *saveptr;

    if (!cmd || !envp)
        return NULL;

    /* If cmd contains a slash, check it directly */
    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return strdup_custom(cmd); /* caller must free */
        return NULL;
    }

    path = _getenv_from_envp(envp, "PATH");
    if (!path)
        return NULL;

    /* duplicate because strtok modifies */
    pathdup = strdup_custom(path);
    if (!pathdup)
        return NULL;

    token = strtok_r(pathdup, ":", &saveptr);
    while (token)
    {
        candidate = join_path(token, cmd);
        if (!candidate)
        {
            token = strtok_r(NULL, ":", &saveptr);
            continue;
        }
        if (access(candidate, X_OK) == 0)
        {
            free(pathdup);
            return candidate; /* caller frees candidate */
        }
        free(candidate);
        candidate = NULL;
        token = strtok_r(NULL, ":", &saveptr);
    }

    free(pathdup);
    return NULL;
}

