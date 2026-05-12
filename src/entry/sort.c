#include "ft_ls.h"

int cmp_alpha(t_entry *a, t_entry *b)
{
    return (ft_strcmp(a->name, b->name));
}

int cmp_mtime(t_entry *a, t_entry *b)
{
    if (a->st.st_mtime != b->st.st_mtime)
        return (a->st.st_mtime > b->st.st_mtime ? -1 : 1);
    if (a->st.st_mtim.tv_nsec != b->st.st_mtim.tv_nsec)
        return (a->st.st_mtim.tv_nsec > b->st.st_mtim.tv_nsec ? -1 : 1);
    return (cmp_alpha(a, b));
}

int cmp_atime(t_entry *a, t_entry *b)
{
    if (a->st.st_atime != b->st.st_atime)
        return (a->st.st_atime > b->st.st_atime ? -1 : 1);
    if (a->st.st_atim.tv_nsec != b->st.st_atim.tv_nsec)
        return (a->st.st_atim.tv_nsec > b->st.st_atim.tv_nsec ? -1 : 1);
    return (cmp_alpha(a, b));
}

/**
 * @brief Sort by size, largest first (descending). Name is the tiebreaker.
 *
 * @param a 
 * @param b 
 * @return 
 */
int cmp_size(t_entry *a, t_entry *b)
{
    if (a->st.st_size != b ->st.st_size)
        return (a->st.st_size > b->st.st_size ? -1 : 1);
    return (cmp_alpha(a, b));
}

/**
    * @brief Get the appropriate comparator based on options.
* @return NULL if -f (no sorting).
*/
t_cmp_fn get_comparator(t_opts *opts)
{
    if (opts->f)
        return (NULL);
    if (opts->cap_s)
        return (cmp_size);
    if (opts->t && opts->u)
        return (cmp_atime);
    if (opts->t)
        return (cmp_mtime);
    return (cmp_alpha);
}

void sort_entries(t_entry *entries, int count, t_cmp_fn cmp, bool reverse)
{
    int i;
    int j;
    t_entry tmp;
    int result;

    if (!cmp || count <= 1)
        return ;
    i = 1;
    while (i < count)
    {
        tmp = entries[i];
        j = i - 1;
        while (j >= 0)
        {
            result = cmp(&entries[j], &tmp);
            if (reverse)
                result = -result;
            if (result <= 0)
                break ;
            entries[j + 1] = entries[j];
            j--;
        }
        entries[j + 1] = tmp;
        i++;
    }
}
