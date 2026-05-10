#include "ft_ls.h"
#include <sys/ioctl.h>

/**
 * @brief Get terminal width with ioctl(TIOCGWINSZ)
 *
 * @return 0 if stdout isn't a terminal - caller falls back to single column.
 */
int get_terminal_width(void)
{
    struct winsize ws;

    if (!isatty(1))
        return (0);
    if (ioctl(1, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
        return (80);
    return (ws.ws_col);
}
