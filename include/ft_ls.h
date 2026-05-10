#ifndef FT_LS_H
# define FT_LS_H

# include <sys/stat.h>
# include <stdbool.h>
# include <dirent.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include "../libft/libft.h"

# define BUF_SIZE 8192

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
    char        *name; // filename only ("foo.c")
    char        *path; // full path ("src/foo.c") - needed for recursion + lstat
    struct stat st; // entire lstat result: mode, size, times, uid, gid, nlink...
    char        *link_target; // readlink result, NULL if not a symlink
    bool        has_xattr; // for the '@' indicator
    bool        has_acl; // for the '+' indicator
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
    char    data[BUF_SIZE]; // flush once per 8kb
    int     len;
}   t_buf;

typedef struct s_col_widths
{
    int nlink;
    int user;
    int group;
    int size;
    int major;
    int minor;
}   t_col_widths;

typedef int (*t_cmp_fn)(t_entry *a, t_entry *b);

typedef struct s_classified
{
    t_entry *files;
    int     file_count;
    t_entry *dirs;
    int     dir_count;
    bool    had_errors;
}   t_classified;

// parse_args.c
int parse_args(int argc, char **argv, t_opts *opts, char ***targets);

// entry.c
int entry_create(t_entry *entry, const char *path, const char *name_override);
void entry_destroy(t_entry *entry);
void entry_array_destroy(t_entry *entries, int count);

// sort.cconst char
int         cmp_alpha(t_entry *a, t_entry *b);
int         cmp_mtime(t_entry *a, t_entry *b);
int         cmp_atime(t_entry *a, t_entry *b);
t_cmp_fn    get_comparator(t_opts *opts);
void        sort_entries(t_entry *entries, int count, t_cmp_fn cmp, bool reverse);

// read_dir.c
t_entry     *read_directory(const char *path, t_opts *opts, int *count);

// cache.c
void        cache_init(t_cache *cache);
void        cache_destroy(t_cache *cache);;
const char  *cache_get_user(t_cache *cache, uid_t uid);
const char  *cache_get_group(t_cache *cache, gid_t gid);

// list_directory.c
void    list_directory(const char *path, t_opts *opts, bool print_header, bool print_separator, t_buf *buf, t_cache *cache);

// classify.c
int     classify_targets(char **targets, int count, t_opts *opts, t_classified *result);

// path_util.c
char    *path_join(const char *dir, const char *name);

// term.c
int get_terminal_width(void);

// buf.c
void    buf_init(t_buf *buf);
void    buf_flush(t_buf *buf);
void    buf_write(t_buf *buf, const char *src, int len);
void    buf_write_str(t_buf *buf, const char *s);
void    buf_write_char(t_buf *buf, char c);
void    buf_write_uint(t_buf *buf, unsigned long n);
void    buf_write_pad(t_buf *buf, char c, int n);
int     uint_width(unsigned long n);

// format_perms.c
void    format_perms(mode_t mode, char *out);

// format_time.c
void format_time(struct stat *st, t_opts *opts, t_buf *buf);

// format_long.c
void format_long_listing(t_entry *entries, int count, t_opts *opts, t_cache *cache, t_buf *buf, bool print_total);

// format_columns.c
void format_columns_listing(t_entry *entries, int count, t_opts *opts, t_buf *buf);

// color.c
const char *color_for_entry(t_entry *entry);
void write_colored_name(t_buf *buf, t_entry *entry, t_opts *opts);

// display.c
void    display_entries(t_entry *entries, int count, t_opts *opts, t_buf *buf, t_cache *cache, bool show_total);

#endif
