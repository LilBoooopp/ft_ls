#include "ft_ls.h"

/**
* @brief Join a directory path and a filename with a '/' separator.
* Handles the case where dir already ends with '/'.
* @return a malloc'd string. NULL on allocation failure.
*/
char *path_join(const char *dir, const char *name)
{
    int dir_len;
    int name_len;
    bool need_slash;
    char *result;
    int i;
    int j;

    dir_len = ft_strlen(dir);
    name_len = ft_strlen(name);
    need_slash = (dir_len > 0 && dir[dir_len - 1] != '/');
    result = malloc(dir_len + (need_slash ? 1 : 0) + name_len + 1);
    if (!result)
        return (NULL);
    i = 0;
    while (i < dir_len)
    {
        result[i] = dir[i];
        i++;
    }
    if (need_slash)
        result[i++] = '/';
    j = 0;
    while (j < name_len)
        result[i++] = name[j++];
    result[i] = '\0';
    return (result);
}
