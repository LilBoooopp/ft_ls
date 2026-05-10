#include "ft_ls.h"

void    display_entries(t_entry *entries, int count, t_opts *opts, t_buf *buf, t_cache *cache, bool show_total)
{
    if (opts->l || opts->g)
    {
        format_long_listing(entries, count, opts, cache, buf, show_total);
        return ;
    }
    (void)cache;
    (void)show_total;
    format_columns_listing(entries, count, opts, buf);
}
