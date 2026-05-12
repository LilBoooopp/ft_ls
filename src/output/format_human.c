#include "ft_ls.h"

# define UNIT_STR " KMGTPE"

static int write_uint_to_buf(unsigned long long n, char *out)
{
    char    digits[24];
    int     dl;
    int     len;

    dl = 0;
    if (n == 0)
        digits[dl++] = '0';
    while (n > 0)
    {
        digits[dl++] = '0' + (n % 10);
        n /= 10;
    }
    len = 0;
    while (dl > 0)
        out[len++] = digits[--dl];
    return (len);
}

/**
 * @brief Compute ceil(size * 10 / divisor) without overflowing 64 bits.
 * Splits size into whole and remainder so we don't multiply huge values by 10.
 *
 * @param size 
 * @param divisor 
 * @return 
 */
static unsigned long long   scaled_tenths(unsigned long long size, unsigned long long divisor)
{
    unsigned long long  whole;
    unsigned long long  rem;
    unsigned long long  frac_tenths;

    whole = size / divisor;
    rem = size % divisor;
    frac_tenths = (rem * 10 + divisor - 1) / divisor;
    return (whole * 10 + frac_tenths);
}

/**
 * @brief Format size in human-readable form (1024-based).
 *
 * @param size 
 * @param out 
 */
int format_human_size(unsigned long long size, char *out)
{
    int                 unit;
    unsigned long long  divisor;
    unsigned long long  whole;
    unsigned long long  tenths;
    int                 len;

    unit = 0;
    divisor = 1;
    while (size / divisor >= 1024 && unit < 6)
    {
        divisor *= 1024;
        unit++;
    }
    whole = (size + divisor - 1) / divisor;
    if (whole >= 1024 && unit < 6)
    {
        divisor *= 1024;
        unit++;
    }
    tenths = scaled_tenths(size, divisor);
    if (tenths < 100 && unit > 0)
    {
        out[0] = '0' + (tenths / 10);
        out[1] = '.';
        out[2] = '0' + (tenths % 10);
        out[3] = UNIT_STR[unit];
        out[4] = '\0';
        return (4);
    }
    len = write_uint_to_buf((size + divisor - 1) / divisor, out);
    if (unit > 0)
        out[len++] = UNIT_STR[unit];
    out[len] = '\0';
    return (len);
}

