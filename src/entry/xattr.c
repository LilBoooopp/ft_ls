#include "ft_ls.h"
#include <sys/xattr.h>

/**
 * @brief Populate entry->has_acl
 * Linux stores ACLS as xattrs in the systm.* namespace:
 *  systme.posix_acl_Access - the file's ACL (any file/dir)
 *  system.posix_acl_default - default ACL (directories only)
 *
 * @param entry 
 */
void entry_load_acl_info(t_entry *entry)
{
    ssize_t r;

    if (S_ISLNK(entry->st.st_mode))
        return ;
    r = getxattr(entry->path, "system.posix_acl_access", NULL, 0);
    if (r >= 0)
    {
        entry->has_acl = true;
        return ;
    }
    if (S_ISDIR(entry->st.st_mode))
    {
        r = getxattr(entry->path, "system.posix_acl_default", NULL, 0);
        if (r >= 0)
            entry->has_acl = true;
    }
}
