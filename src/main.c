#include "../libft/libft.h"
#include "../include/ft_ls.h"

/**
* @brief Decide whether each directory needs a "path:" header.
* Headers are needed when:
*     - We're recursing (-R), OR
*     - More than one "section" is being printed (multiple dirs, or files+dirs)
*/
static bool needs_headers(t_opts *opts, t_classified *cl)
{
    int sections;

    if (opts->cap_r)
        return (true);
    sections = cl->dir_count;
    if (cl->file_count > 0)
        sections++;
    return (sections > 1);
}

int main(int argc, char **argv)
{
    t_opts          opts;
    char            **targets;
    int             count;
    t_classified    cl;
    t_buf           buf;
    t_cache         cache; 
    bool            print_header;
    bool            separator;
    int             i;

    count = parse_args(argc, argv, &opts, &targets);
    if (count == -1)
        return (2);
    if (classify_targets(targets, count, &opts, &cl) == -1)
    {
        free(targets);
        return (1);
    }
    buf_init(&buf);
    cache_init(&cache);
    separator = false;
    if (cl.file_count > 0)
    {
        display_entries(cl.files, cl.file_count, &opts, &buf, &cache, false);
        separator = true;
    }
    print_header = needs_headers(&opts, &cl);
    i = 0;
    while (i < cl.dir_count)
    {
        list_directory(cl.dirs[i].path, &opts, print_header, separator, &buf, &cache);
        separator = true;
        i++;
    }
    buf_flush(&buf);
    cache_destroy(&cache);
    entry_array_destroy(cl.files, cl.file_count);
    entry_array_destroy(cl.dirs, cl.dir_count);
    free(targets);
    return (cl.had_errors ? 2 : 0);
}
