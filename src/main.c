#include "../libft/libft.h"
#include "../include/ft_ls.h"

static void debug_classified(t_classified *cl)
{
    int i;

    ft_putstr_fd("--- Files ---\n", 1);
    i = 0;
    while (i < cl->file_count)
    {
        ft_putstr_fd("  ", 1);
        ft_putstr_fd(cl->files[i].path, 1);
        ft_putstr_fd("\n", 1);
        i++;
    }
    ft_putstr_fd("--- Directories ---\n", 1);
    i = 0;
    while (i < cl->dir_count)
    {
        ft_putstr_fd("  ", 1);
        ft_putstr_fd(cl->dirs[i].path, 1);
        ft_putstr_fd("\n", 1);
        i++;
    }
}

int main(int argc, char **argv)
{
    t_opts  opts;
    char    **targets;
    int     count;
    t_classified cl;

    count = parse_args(argc, argv, &opts, &targets);
    if (count == -1)
        return (2);
    if (classify_targets(targets, count, &opts, &cl) == -1)
    {
        free(targets);
        return (1);
    }
    debug_classified(&cl);
    entry_array_destroy(cl.files, cl.file_count);
    entry_array_destroy(cl.dirs, cl.dir_count);
    free(targets);
    return (cl.had_errors ? 2 : 0);
}
