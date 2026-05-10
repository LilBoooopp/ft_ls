#include "ft_ls.h"
#include <time.h>

# define HALF_YEAR_SECS 15778476L

/**
* @brief Choose which time field to display based on options.
* Default is mtime (modification). -u uses atime (access).
*/
static time_t pick_time(struct stat *st, t_opts *opts)
{
    if (opts->u)
        return (st->st_atime);
    return (st->st_mtime);
}

/**
* @brief get month + day, HH:MM and year.
* If within 6 months:   "Jun 1 14:23"
* Else :                "Jun 1  2024"
* Both 12 char
*/
void format_time(struct stat *st, t_opts *opts, t_buf *buf)
{
    time_t t;
    time_t now;
    char *str;
    bool recent;

    t = pick_time(st, opts);
    now = time(NULL);
    recent = (t <= now && (now - t) < HALF_YEAR_SECS);
    if (t > now)
        recent = ((t - now) < HALF_YEAR_SECS);
    str = ctime(&t);
    if (!str)
    {
        buf_write_pad(buf, ' ', 12);
        return ;
    }
    buf_write(buf, &str[4], 7);
    if (recent)
        buf_write(buf, &str[11], 5);
    else
    {
        buf_write_char(buf, ' ');
        buf_write(buf, &str[20], 4);
    }
}

