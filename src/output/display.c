#include "ft_ls.h"

void    display_entries(t_entry *entries, int count, t_opts *opts)
{
    int i;

    (void)opts;
    i = 0;
    while (i < count)
    {
        ft_putstr_fd(entries[i].name, 1);
        ft_putstr_fd("\n", 1);
        i++;
    }
}
