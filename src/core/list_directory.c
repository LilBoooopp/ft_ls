#include "ft_ls.h"
#include <errno.h>
#include <stdbool.h>

static void print_open_error(const char *path)
{
    char *msg;

    msg = strerror(errno);
    ft_putstr_fd("ft_ls: cannot open director '", 2);
    ft_putstr_fd((char *)path, 2);
    ft_putstr_fd("': ", 2);
    ft_putstr_fd(msg, 2);
    ft_putstr_fd("\n", 2);
}

/**
* @brief Skip "." and ".." for recursion purposes.
*/
static bool is_dot_or_dotdot(const char *name)
{
    if (name[0] != '.')
        return (false);
    if (name[1] == '\0')
        return (true);
    if (name[1] == '.' && name[2] == '\0')
        return (true);
    return (false);
}

/**
* @brief Recurse into all subdirectories of the just-listed directory.
* Skip symlinks-to-directories (real ls doesn't recurse into them with -R).
*/
static void recurse_into_subdirs(t_entry *entries, int count, t_opts *opts)
{
    int i;

    i = 0;
    while (i < count)
    {
        if (S_ISDIR(entries[i].st.st_mode) && !is_dot_or_dotdot(entries[i].name))
            list_directory(entries[i].path, opts, true, true);
        i++;
    }
}

/**
* @brief List a single directory.
*   print_header - true means print "path:" line (mutiple targets, or -R)
*   print_separator - true means print blank line before this section
*/
void list_directory(const char *path, t_opts *opts, bool print_header, bool print_separator)
{
    t_entry *entries;
    int count;
    t_cmp_fn cmp;

    entries = read_directory(path, opts, &count);
    if (!entries)
    {
        print_open_error(path);
        return ;
    }
    if (print_separator)
        ft_putstr_fd("\n", 1);
    if (print_header)
    {
        ft_putstr_fd((char *)path, 1);
        ft_putstr_fd(":\n", 1);
    }
    cmp = get_comparator(opts);
    sort_entries(entries, count, cmp, opts->r);
    display_entries(entries, count, opts);
    if (opts->cap_r)
        recurse_into_subdirs(entries, count, opts);
    entry_array_destroy(entries, count);
}
