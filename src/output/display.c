#include "ft_ls.h"

void    display_entries(t_entry *entries, int count, t_opts *opts, t_buf *buf)
{
    int i;

    (void)opts;
    i = 0;
    while (i < count)
    {
        buf_write_str(buf, entries[i].name);
        buf_write_char(buf, '\n');
        i++;
    }
}
