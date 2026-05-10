#include "ft_ls.h"

void    display_entries(t_entry *entries, int count, t_opts *opts, t_buf *buf, t_cache *cache, bool show_total)
{
    int i;

    if (opts->l || opts->g)
    {
        format_long_listing(entries, count, opts, cache, buf, show_total);
    }
    i = 0;
    while (i < count)
    {
        buf_write_str(buf, entries[i].name);
        buf_write_char(buf, '\n');
        i++;
    }
}
