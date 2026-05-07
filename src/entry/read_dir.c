#include "ft_ls.h"

/**
* @brief Grow a t_entry array, doubling capcity.
* @return 0 on success -1 on allocation failure.
*/
static int grow_entries(t_entry **entries, int *cap)
{
    int new_cap;
    t_entry *new_arr;
    int i;

    new_cap = (*cap == 0) ? 16 : *cap * 2;
    new_arr = malloc(sizeof(t_entry) * new_cap);
    if (!new_arr)
        return (-1);
    i = 0;
    while (i < *cap)
    {
        new_arr[i] = (*entries)[i];
        i++;
    }
    free(*entries);
    *entries = new_arr;
    *cap = new_cap;
    return (0);
}

/**
* @brief Decide if an entry should be included in the listing:
*   Without -a: skip anything starting with '.'
*   With -a: include everything (including . and ..)
*/
static bool should_include(const char *name, t_opts *opts)
{
    if (opts->a)
        return (true);
    return (name[0] != '.');
}

t_entry *read_directory(const char *path, t_opts *opts, int *count)
{
    DIR *dir;
    struct dirent *de;
    t_entry *entries;
    int cap;
    char *full_path;

    dir = opendir(path);
    if (!dir)
        return (NULL);
    cap = 16;
    entries = malloc(sizeof(t_entry) * cap);
    if (!entries)
    {
        closedir(dir);
        return (NULL);
    }
    *count = 0;
    while ((de = readdir(dir)) != NULL)
    {
        if (!should_include(de->d_name, opts))
            continue ;
        if (*count >= cap && grow_entries(&entries, &cap) == -1)
            break ;
        full_path = path_join(path, de->d_name);
        if (!full_path)
            break ;
        if (entry_create(&entries[*count], full_path, de->d_name) == 0)
            (*count)++;
        free(full_path);
    }
    closedir(dir);
    return (entries);
}
