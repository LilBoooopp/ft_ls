#include "../libft/libft.h"
#include "../include/ft_ls.h"

static void debug_opts(t_opts *opts, char **targets, int count)
{
    int i;

    write(1, "Options:\n", 9);
    if (opts->l) write(1, "  -l\n", 5);
    if (opts->cap_r) write(1, "  -R\n", 5);
    if (opts->a) write(1, "  -a\n", 5);
    if (opts->r) write(1, "  -r\n", 5);
    if (opts->t) write(1, "  -t\n", 5);
    if (opts->u) write(1, "  -u\n", 5);
    if (opts->f) write(1, "  -f\n", 5);
    if (opts->g) write(1, "  -g\n", 5);
    if (opts->d) write(1, "  -d\n", 5);
    if (opts->color) write(1, " -G\n", 5);
    write(1, "Targets:\n", 9);
    i = 0;
    while (i < count)
    {
        write(1, "  ", 2);
        write(1, targets[i], ft_strlen(targets[i]));
        write(1, "\n", 1);
        i++;
    }
}

int main(int argc, char **argv)
{
    t_opts  opts;
    char    **targets;
    int     count;

    count = parse_args(argc, argv, &opts, &targets);
    if (count == -1)
        return (2);
    debug_opts(&opts, targets, count);
    free(targets);
    return (0);
}
