#include "ft_ls.h"
#include <sys/stat.h>

static char file_type_char(mode_t mode)
{
    if (S_ISDIR(mode))
        return ('d');
    if (S_ISLNK(mode))
        return ('l');
    if (S_ISCHR(mode))
        return ('c');
    if (S_ISBLK(mode))
        return ('b');
    if (S_ISFIFO(mode))
        return ('p');
    if (S_ISSOCK(mode))
        return ('s');
    return ('-');
}

/**
* @brief Build the rwx triplet for one o user/group/other.
* special_bit: S_ISUID for user, S_ISGID for group, S_ISVTX for other.
* special_char: 's' for setuid/setgid, 't' for sticky.
*/
static void build_triplet(char *out, mode_t mode, mode_t r, mode_t w, mode_t x, mode_t special_bit, char special_char)
{
    out[0] = (mode & r) ? 'r' : '-';
    out[1] = (mode & w) ? 'w' : '-';
    if (mode & special_bit)
        out[2] = (mode & x) ? special_char : (special_char - 32);
    else
        out[2] = (mode & x) ? 'x' : '-';
}

/**
* @brief Fill a 10-char permissions string (no null terminator).
* Format: tugogoo where t=type, u/g/o are rwx triplets.
*/
void format_perms(mode_t mode, char *out)
{
    out[0] = file_type_char(mode);
    build_triplet(&out[1], mode, S_IRUSR, S_IWUSR, S_IXUSR, S_ISUID, 's');
    build_triplet(&out[4], mode, S_IRGRP, S_IWGRP, S_IXGRP, S_ISGID, 's');
    build_triplet(&out[7], mode, S_IROTH, S_IWOTH, S_IXOTH, S_ISVTX, 't');
}

