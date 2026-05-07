#ifndef FT_LS_H
# define FT_LS_H

# include <sys/stat.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

typedef struct s_opts
{
    bool    l; // long format
    bool    cap_r;
    bool    a;
    bool    r; //  reverse sort
    bool    t; //  sort by time
    bool    u; //  sort/show access time
    bool    f; //  unsorted, implies -a
    bool    g; //  like -l but no owner
    bool    d; //  list directories themselves, not contents
    bool    color; //  colorized output
}   t_opts;

typedef struct s_entry
{
    char    *name; // filename only ("foo.c")
    char    *path; // full path ("src/foo.c") - needed for recursion + lstat
    struct stat st; // entire lstat result: mode, size, times, uid, gid, nlink...
    char    *link_target; // readlink result, NULL if not a symlink
    bool    hasxattr; // for the '@' indicator
    bool    has_acl; // for the '+' indicator
}   t_entry;

typedef struct s_cache_entry
{
    id_t    id;
    char    *name;
}   t_cache_entry;

typedef struct s_cache
{
    t_cache_entry   *users;
    int             user_count;
    int             user_cap;
    t_cache_entry   *groups;
    int             group_count;
    int             group_cap;
}   t_cache;

typedef struct s_buf
{
    char    data[8192]; // flush once per 8kb
    int     len;
}   t_buf;

typedef struct s_col_widths
{
    int nlink;
    int user;
    int group;
    int size;
}   t_col_widths;

int parse_args(int argc, char **argv, t_opts *opts, char ***targets);

#endif
