#include "ft_ls.h"
#include <sys/stat.h>
#include <sys/sysmacros.h>


/**
* @brief Tracks widths for regular size, major and minor separately.
* After scanning, the size column needs to fit either the widest regular size or "MAJ, MIN" with right-aligned major and minor - whichever wins.
*/
static void scan_size(t_entry *entry, t_opts *opts, t_col_widths *w)
{
    int cur;
    char human[8];

    if (S_ISCHR(entry->st.st_mode) || S_ISBLK(entry->st.st_mode))
    {
        cur = uint_width(major(entry->st.st_rdev));
        if (cur > w->major)
            w->major = cur;
        cur = uint_width(minor(entry->st.st_rdev));
        if (cur > w->minor)
            w->minor = cur;
    }
    else
    {
        if (opts->h)
            cur = format_human_size(entry->st.st_size, human);
        else
            cur = uint_width(entry->st.st_size);
        cur = uint_width(entry->st.st_size);
        if (cur > w->size)
            w->size = cur;
    }
}

static void compute_widths(t_entry *entries, int count, t_opts *opts, t_cache *cache, t_col_widths *w)
{
    int i;
    int cur;
    const char *name;

    w->nlink = 1;
    w->user = 1;
    w->group = 1;
    w->size = 1;
    w->major = 0;
    w->minor = 0;
    w->any_acl = false;
    i = 0;
    while (i < count)
    {
        entry_load_acl_info(&entries[i]);
        if (entries[i].has_acl)
            w->any_acl = true;
        cur = uint_width(entries[i].st.st_nlink);
        if (cur > w->nlink)
            w->nlink = cur;
        name = cache_get_user(cache, entries[i].st.st_uid);
        if (name && (cur = ft_strlen(name)) > w->user)
            w->user = cur;
        name = cache_get_group(cache, entries[i].st.st_gid);
        if (name && (cur = ft_strlen(name)) > w->group)
            w->group = cur;
        scan_size(&entries[i], opts, w);
        i++;
    }
    if (w->major > 0 || w-> minor > 0)
    {
        cur = w->major + 2 + w->minor;
        if (cur > w->size)
            w->size = cur;
    }
}

static void write_total_line(t_entry *entries, int count, t_opts *opts, t_buf *buf)
{
    unsigned long total_blocks;
    char          human[8];
    int           len;
    int           i;

    total_blocks = 0;
    i = 0;
    while (i < count)
        total_blocks += entries[i++].st.st_blocks;
    buf_write_str(buf, "total ");
    if (opts->h)
    {
        len = format_human_size(total_blocks * 512, human);
        buf_write(buf, human, len);
    }
    else
        buf_write_uint(buf, total_blocks / 2);
    buf_write_char(buf, '\n');
}

/**
* @brief Device size column: pad to size_width, then major right-aligned to major_w, ", ", minor right-aligned to minor_w.
*/
static void write_device_size(t_entry *entry, t_col_widths *w, t_buf *buf)
{
    unsigned int maj;
    unsigned int min;
    int dev_total;

    maj = major(entry->st.st_rdev);
    min = minor(entry->st.st_rdev);
    dev_total = w->major + 2 + w->minor;
    buf_write_pad(buf, ' ', w->size - dev_total);
    buf_write_pad(buf, ' ', w->major - uint_width(maj));
    buf_write_uint(buf, maj);
    buf_write_str(buf, ", ");
    buf_write_pad(buf, ' ', w->minor - uint_width(min));
    buf_write_uint(buf, min);
}

static void write_size_col(t_entry *entry, t_opts *opts, t_col_widths *w, t_buf *buf)
{
    char    human[8];
    int     len;

    if (S_ISCHR(entry->st.st_mode) || S_ISBLK(entry->st.st_mode))
    {
        write_device_size(entry, w, buf);
        return ;
    }
    if (opts->h)
    {
        len = format_human_size(entry->st.st_size, human);
        buf_write_pad(buf, ' ', w->size - len);
        buf_write(buf, human, len);
        return ;
    }
    buf_write_pad(buf, ' ', w->size - uint_width(entry->st.st_size));
    buf_write_uint(buf, entry->st.st_size);
}

static void write_long_line(t_entry *entry, t_opts *opts, t_cache *cache, t_col_widths *w, t_buf *buf)
{
    char perms[10];
    const char *name;
    int len;

    format_perms(entry->st.st_mode, perms);
    buf_write(buf, perms, 10);
    if (entry->has_acl)
        buf_write_char(buf, '+');
    else if (w->any_acl)
        buf_write_char(buf, ' ');
    buf_write_char(buf, ' ');
    buf_write_pad(buf, ' ', w->nlink - uint_width(entry->st.st_nlink));
    buf_write_uint(buf, entry->st.st_nlink);
    buf_write_char(buf, ' ');
    name = cache_get_user(cache, entry->st.st_uid);
    len = name ? ft_strlen(name) : 0;
    if (name)
        buf_write_str(buf, name);
    buf_write_pad(buf, ' ', w->user - len + 1);
    name = cache_get_group(cache, entry->st.st_gid);
    len = name ? ft_strlen(name) : 0;
    if (name)
        buf_write_str(buf, name);
    buf_write_pad(buf, ' ', w->group - len + 1);
    write_size_col(entry, opts, w, buf);
    buf_write_char(buf, ' ');
    format_time(&entry->st, opts, buf);
    buf_write_char(buf, ' ');
    write_colored_name(buf, entry, opts);
    if (S_ISLNK(entry->st.st_mode) && entry->link_target)
    {
        buf_write_str(buf, " -> ");
        buf_write_str(buf, entry->link_target);
    }
    buf_write_char(buf, '\n');
}

void format_long_listing(t_entry *entries, int count, t_opts *opts, t_cache *cache, t_buf *buf, bool print_total)
{
    t_col_widths w;
    int i;

    compute_widths(entries, count, opts, cache, &w);
    if (print_total)
        write_total_line(entries, count, opts, buf);
    i = 0;
    while (i < count)
    {
        write_long_line(&entries[i], opts, cache, &w, buf);
        i++;
    }
}

