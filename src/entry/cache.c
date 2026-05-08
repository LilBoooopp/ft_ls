#include "ft_ls.h"
#include <pwd.h>
#include <grp.h>

# define CACHE_INIT_CAP 8

void cache_init(t_cache *cache)
{
    cache->users = NULL;
    cache->user_count = 0;
    cache->user_cap = 0;
    cache->groups = NULL;
    cache->group_count = 0;
    cache->group_cap = 0;
}

static void free_entries(t_cache_entry *arr, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        free(arr[i].name);
        i++;
    }
    free(arr);
}

void cache_destroy(t_cache *cache)
{
    free_entries(cache->users, cache->user_count);
    free_entries(cache->groups, cache->group_count);
}

/**
* @brief Convert an unsigned id to its decimal string form.
* Used as a fallback when getpwuid/getgrgid fails.
*/
static char *id_to_str(unsigned long id)
{
    char digits[24];
    int i;
    char *result;
    int j;

    i = 0;
    if (id == 0)
        digits[i++] = '0';
    else
    {
        while (id > 0)
        {
            digits[i++] = '0' + (id % 10);
            id /= 10;
        }
    }
    result = malloc(i + 1);
    if (!result)
        return (NULL);
    j = 0;
    while (i > 0)
        result[j++] = digits[--i];
    result[j] = '\0';
    return (result);
}

static int grow_cache(t_cache_entry **arr, int *cap)
{
    int new_cap;
    t_cache_entry *new_arr;
    int i;

    new_cap = (*cap == 0) ? CACHE_INIT_CAP : *cap * 2;
    new_arr = malloc(sizeof(t_cache_entry) * new_cap);
    if (!new_arr)
        return (-1);
    i = 0;
    while (i < *cap)
    {
        new_arr[i] = (*arr)[i];
        i++;
    }
    free(*arr);
    *arr = new_arr;
    *cap = new_cap;
    return (0);
}

/**
 * @brief Generic cache lookup: scan for id, fetch via callbak if missing, insert and return.
 * fetch returns a malloc'd string (the cache takes ownership).
 *
 * @param arr 
 * @param count 
 * @param cap 
 * @param id 
 * @param fetch 
 * @return 
 */
static const char *cache_lookup(t_cache_entry **arr, int *count, int *cap, id_t id, char *(*fetch)(id_t))
{
    int i;
    char *name;

    i = 0;
    while (i < *count)
    {
        if ((*arr)[i].id == id)
            return ((*arr)[i].name);
        i++;
    }
    name = fetch(id);
    if (!name)
        return (NULL);
    if (*count >= *cap && grow_cache(arr, cap) == -1)
        return (free(name), NULL);
    (*arr)[*count].id = id;
    (*arr)[*count].name = name;
    (*count)++;
    return (name);
}

static char *fetch_user(id_t id)
{
    struct passwd *pw;

    pw = getpwuid(id);
    if (pw)
        return (ft_strdup(pw->pw_name));
    return (id_to_str(id));
}

static char *fetch_group(id_t id)
{
    struct group *gr;

    gr = getgrgid(id);
    if (gr)
        return (ft_strdup(gr->gr_name));
    return (id_to_str(id));
}

const char *cache_get_user(t_cache *cache, uid_t uid)
{
    return (cache_lookup(&cache->users, &cache->user_count, &cache->user_cap, uid, fetch_user));
}

const char *cache_get_group(t_cache *cache, gid_t gid)
{
    return (cache_lookup(&cache->groups, &cache->group_count, &cache->group_cap, gid, fetch_group));
}

