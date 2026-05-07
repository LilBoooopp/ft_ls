#include "ft_ls.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

/**
* @brief Sort an array of strings alphabetically (for error messages).
*/
static void sort_strings(char **arr, int count)
{
    int i;
    int j;
    char *tmp;

    i = 1;
    while (i < count)
    {
        tmp = arr[i];
        j = i - 1;
        while (j >= 0 && ft_strcmp(arr[j], tmp) > 0)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = tmp;
        i++;
    }
}

static void print_errors(char **errors, int *errnos, int count)
{
    int i;
    char *name;
    char *msg;

    sort_strings(errors, count);
    i = 0;
    while (i < count)
    {
        name = errors[i];
        msg = strerror(errnos[i]);
        write(2, "ft_ls: cannot access '", 22);
        write(2, name, ft_strlen(name));
        write(2, "': ", 3);
        write(2, msg, strlen(msg));
        write(2, "\n", 1);
        i++;
    }
}

int classify_targets(char **targets, int count, t_opts *opts, t_classified *result)
{
    struct stat st;
    int i;
    char **errors;
    int *errnos;
    int err_count;
    t_cmp_fn cmp;

    result->files = malloc(sizeof(t_entry) * count);
    result->dirs = malloc(sizeof(t_entry) * count);
    errors = malloc(sizeof(t_entry) * count);
    errnos = malloc(sizeof(int) * count);
    if (!result->files || !result->dirs || !errors || !errnos)
    {
        free(result->files);
        free(result->dirs);
        free(errors);
        free(errnos);
        return (-1);
    }
    result->file_count = 0;
    result->dir_count = 0;
    err_count = 0;
    i = 0;
    while (i < count)
    {
        if (lstat(targets[i], &st) == -1)
        {
            errnos[err_count] = errno;
            errors[err_count++] = targets[i];
        }
        else if (S_ISDIR(st.st_mode) && !opts->d)
        {
            if (entry_create(&result->dirs[result->dir_count], targets[i], NULL) == 0)
                result->dir_count++;
        }
        else
        {
            if (entry_create(&result->files[result->file_count], targets[i], NULL) == 0)
                result->file_count++;
        }
        i++;
    }
    if (err_count > 0)
    {
        print_errors(errors, errnos, err_count);
        result->had_errors = true;
    }
    else
        result->had_errors = false;
    free(errors);
    free(errnos);
    cmp = get_comparator(opts);
    sort_entries(result->files, result->file_count, cmp, opts->r);
    sort_entries(result->dirs, result->dir_count, cmp, opts->r);
    return (0);
}

