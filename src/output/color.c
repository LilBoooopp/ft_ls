#include "ft_ls.h"
#include <sys/stat.h>

# define COLOR_RESET        "\033[0m"
# define COLOR_DIR          "\033[01;34m"
# define COLOR_LINK         "\033[01;36m"
# define COLOR_ORPHAN       "\033[01;31m"
# define COLOR_EXEC         "\033[01;32m"
# define COLOR_FIFO         "\033[m"
# define COLOR_SOCK         "\033[01;35m"
# define COLOR_BLK          "\033[01;33m"
# define COLOR_CHR          "\033[01;33m"
# define COLOR_SETUID       "\033[37;41m"
# define COLOR_SETGID       "\033[30;43m"
# define COLOR_STICKY       "\033[37;44m"
# define COLOR_OW           "\033[34;42m"
# define COLOR_STICKY_OW    "\033[30;42m"


/**
 * @brief Directory colors depend on sticky bit and other-writable bit.
 *
 * @param m 
 * @return 
 */
static const char   *color_for_dir(mode_t m)
{
    if ((m & S_ISVTX) && (m & S_IWOTH))
        return (COLOR_STICKY_OW);
    if (m & S_ISVTX)
        return (COLOR_STICKY);
    if (m & S_IWOTH)
        return (COLOR_OW);
    return (COLOR_DIR);
}

/**
 * @brief Symlink colors: cyan for valid, red for broken (target doesn't exist)
 * stat() follows the link; if it fails, the target is missing.
 *
 * @param path 
 * @return 
 */
static const char   *color_for_link(const char *path)
{
    struct stat target_st;

    if (stat(path, &target_st) == -1)
        return (COLOR_ORPHAN);
    return (COLOR_LINK);
}

/**
 * @brief Pick a color based on file type and special bits.
 *
 * @param entry 
 * @return NULL when no color applies (regular non-exec file).
 */
const char *color_for_entry(t_entry *entry)
{
    mode_t  m;

    m = entry->st.st_mode;
    if (S_ISLNK(m))
        return (color_for_link(entry->path));
    if (S_ISDIR(m))
        return (color_for_dir(m));
    if (S_ISFIFO(m))
        return (COLOR_SOCK);
    if (S_ISBLK(m))
        return (COLOR_BLK);
    if (S_ISCHR(m))
        return (COLOR_CHR);
    if (m & S_ISUID)
        return (COLOR_SETUID);
    if (m & S_ISGID)
        return (COLOR_SETGID);
    if (m & (S_IXUSR | S_IXGRP | S_IXOTH))
        return (COLOR_EXEC);
    return (NULL);
}

/**
 * @brief Write the entry name, wrapped in color codes if -G is set and color applies.
 *
 * @param buf 
 * @param entry 
 * @param opts 
 */
void write_colored_name(t_buf *buf, t_entry *entry, t_opts *opts)
{
    const char *color;

    if (!opts->color)
    {
        buf_write_str(buf, entry->name);
        return ;
    }
    color = color_for_entry(entry);
    if (!color)
    {
        buf_write_str(buf, entry->name);
        return ;
    }
    buf_write_str(buf, color);
    buf_write_str(buf, entry->name);
    buf_write_str(buf, COLOR_RESET);
}

