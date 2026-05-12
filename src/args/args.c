#include "../../include/ft_ls.h"

static void print_usage(char c)
{
    char    *msg;
    char    buf[64];
    int     i;

    msg = "ft_ls: invalid option -- '";
    i = 0;
    while (msg[i])
    {
        buf[i] = msg[i];
        i++;
    }
    buf[i++] = c;
    buf[i++] = '\'';
    buf[i++] = '\n';
    write(2, buf, i);
}

static int  set_option(char c, t_opts *opts)
{
    if (c == 'l')
        opts->l = true;
    else if (c == 'R')
        opts->cap_r = true;
    else if (c == 'a')
        opts->a = true;
    else if (c == 'r')
        opts->r = true;
    else if (c == 't')
        opts->t = true;
    else if (c == 'u')
        opts->u = true;
    else if (c == 'f')
    {
        opts->f = true;
        opts->a = true;
    }
    else if (c == 'g')
        opts->g = true;
    else if (c == 'd')
        opts->d = true;
    else if (c == 'h')
            opts->h = true;
    else if (c == 'G')
        opts->color = true;
    else
    {
        print_usage(c);
        return (-1);
    }
    return (0);
}

static int  parse_option_str(char *str, t_opts *opts)
{
    int i;

    i = 1;
    while (str[i])
    {
        if (set_option(str[i], opts) == -1)
            return (-1);
        i++;
    }
    return (0);
}

/**
    * Collect non-option arguments into a malloc'd array of pointers into argv
    * If no targets are givem defaults to ".".
    * @returns the count, sets *targets.
*/
static int collect_targets(int argc, char **argv, int start, char ***targets)
{
    int count;
    int i;
    char **result;

    count = argc - start;
    if (count <= 0)
        count = 1;
    result = malloc(sizeof(char *) * (count + 1));
    if (!result)
        return (-1);
    if (start >= argc)
    {
        result[0] = ".";
        result[1] = NULL;
        *targets = result;
        return (1);
    }
    i = 0;
    while (start + i < argc)
    {
        result[i] = argv[start + i];
        i++;
    }
    result[i] = NULL;
    *targets = result;
    return (i);
}

/**
    * Parse all arguments. Options come first (anything starting with '-'),
    * until we hit "--" or a non-option argument.
    * Returns the number of targets on success, -1 on error.
*/
int parse_args(int argc, char **argv, t_opts *opts, char ***targets)
{
    int i;

    *opts = (t_opts){0};
    i = 1;
    while (i < argc)
    {
        if (argv[i][0] == '-' && argv[i][1] != '\0')
        {
            if (argv[i][1] == '-' && argv[i][2] == '\0')
            {
                i++;
                break ;
            }
            if (parse_option_str(argv[i], opts) == -1)
                return (-1);
        }
        else
            break ;
        i++;
    }
    return (collect_targets(argc, argv, i, targets));
}
