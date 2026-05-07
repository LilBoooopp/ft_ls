#include "ft_ls.h"
#include <sys/stat.h>

/**
    * @brief Build a t_entry from a path
* @detail For command-line targets: name and path are the same ("foo" or "/usr/bin").
* For directory contents: name is "foo.c", path is "src/foo.c".
* If name_override is NULL, name is derived from path.
*/
int entry_create(t_entry *entry, const char *path, const char *name_override)
{
    entry->name = ft_strdup(name_override ? name_override : path);
    if (!entry->name)
        return (-1);
    entry->path = ft_strdup(path);
    if (!entry->path)
    {
        free(entry->name);
        return (-1);
    }
    if (lstat(path, &entry->st) == -1)
    {
        free(entry->name);
        free(entry->path);
        return (-1);
    }
    entry->link_target = NULL;
    entry->has_xattr = false;
    entry->has_acl = false;
    if (S_ISLNK(entry->st.st_mode))
    {
        entry->link_target = malloc(1024);
        if (entry->link_target)
        {
            int len = readlink(path, entry->link_target, 1023);
            if (len == -1)
            {
                free(entry->link_target);
                entry->link_target = NULL;
            }
            else
                entry->link_target[len] = '\0';
        }
    }
    return (0);
}

void entry_destroy(t_entry *entry)
{
    free(entry->name);
    free(entry->path);
    free(entry->link_target);
}

void entry_array_destroy(t_entry *entries, int count)
{
    int i;

    i = 0;
    while (i < count)
        entry_destroy(&entries[i++]);
    free(entries);
}

