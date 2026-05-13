#include "ft_ls.h"

/**
 * @brief Compute per-column widths for a given (rows, cols) layout.
 *
 * @param name_widths 
 * @param count 
 * @param rows 
 * @param cols 
 * @param col_widths 
 * @return total width including 2-space separators between columns.
 */
static int compute_layout(int *name_widths, int count, int rows, int cols, int *col_widths)
{
    int i;
    int c;
    int total;

    c = 0;
    while (c < cols)
        col_widths[c++] = 0;
    i = 0;
    while (i < count)
    {
        c = i / rows;
        if (name_widths[i] > col_widths[c])
            col_widths[c] = name_widths[i];
        i++;
    }
    total = 0;
    c = 0;
    while (c < cols)
    {
        total += col_widths[c];
        if (c < cols - 1)
            total += 2;
        c++;
    }
    return (total);
}

/**
 * @brief Find the minimum number of rows that fits in term_width.
 *
 * @param name_widths 
 * @param count 
 * @param term_width 
 * @param col_widths_out 
 * @param cols_out 
 */
static int find_best_rows(int *name_widths, int count, int term_width, int *col_widths_out, int *cols_out)
{
    int rows;
    int cols;
    int total;

    rows = 1;
    while (rows <= count)
    {
        cols = (count + rows - 1) / rows;
        total = compute_layout(name_widths, count, rows, cols, col_widths_out);
        if (total <= term_width)
        {
            *cols_out = cols;
            return (rows);
        }
        rows++;
    }
    *cols_out = 1;
    col_widths_out[0] = 0;
    return (count);
}

static int *compute_name_widths(t_entry *entries, int count)
{
    int *widths;
    int i;

    widths = malloc(sizeof(int) * count);
    if (!widths)
        return (NULL);
    i = 0;
    while (i < count)
    {
        widths[i] = ft_strlen(entries[i].name);
        i++;
    }
    return (widths);
}

static int compute_inode_width(t_entry *entries, int count)
{
    int w;
    int cur;
    int i;

    w = 0;
    i = 0;
    while (i < count)
    {
        cur = uint_width(entries[i].st.st_ino);
        if (cur > w)
            w = cur;
        i++;
    }
    return (w);
}

static void write_inode_prefix(t_entry *entry, int inode_width, t_buf *buf)
{
    buf_write_pad(buf, ' ', inode_width - uint_width(entry->st.st_ino));
    buf_write_uint(buf, entry->st.st_ino);
    buf_write_char(buf, ' ');
}

static void print_grid(t_entry *entries, int count, int rows, int cols, int *col_widths, int *name_widths, int inode_width, t_opts *opts, t_buf *buf)
{
    int r;
    int c;
    int idx;

    r = 0;
    while (r < rows)
    {
        c = 0;
        while (c < cols)
        {
            idx = c * rows + r;
            if (idx >= count)
                break ;
            if (opts->i)
                write_inode_prefix(&entries[idx], inode_width, buf);
            write_colored_name(buf, &entries[idx], opts);
            if (c + 1 < cols && (c + 1) * rows + r < count)
                buf_write_pad(buf, ' ', col_widths[c] - name_widths[idx] + 2);
            c++;
        }
        buf_write_char(buf, '\n');
        r++;
    }
}

/**
 * @brief Single-column fallback: one entry per line. For when stdout is piped or when there's only one enrty.
 *
 * @param entries 
 * @param count 
 * @param opts 
 * @param buf 
 */
static void print_single_column(t_entry *entries, int count, int inode_width, t_opts *opts, t_buf *buf)
{
    int i;

    i = 0;
    while (i < count)
    {
        if (opts->i)
            write_inode_prefix(&entries[i], inode_width, buf);
        write_colored_name(buf, &entries[i], opts);
        buf_write_char(buf, '\n');
        i++;
    }
}

void format_columns_listing(t_entry *entries, int count, t_opts *opts, t_buf *buf)
{
    int term_width;
    int *name_widths;
    int *col_widths;
    int rows;
    int cols;
    int inode_width;
    int i;

    if (count == 0)
        return ;
    inode_width = 0;
    if (opts->i)
        inode_width = compute_inode_width(entries, count);
    term_width = get_terminal_width();
    if (term_width == 0)
    {
        print_single_column(entries, count, inode_width, opts, buf);
        return ;
    }
    name_widths = compute_name_widths(entries, count);
    col_widths = malloc(sizeof(int) * count);
    if (!name_widths || !col_widths)
    {
        free(name_widths);
        free(col_widths);
        print_single_column(entries, count, inode_width, opts, buf);
        return ;
    }
    if (opts->i)
    {
        i = 0;
        while (i < count)
            name_widths[i++] += inode_width + 1;
    }
    rows = find_best_rows(name_widths, count, term_width, col_widths, &cols);
    print_grid(entries, count, rows, cols, col_widths, name_widths, inode_width, opts, buf);
    free(name_widths);
    free(col_widths);
}

