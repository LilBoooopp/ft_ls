#include "ft_ls.h"

void buf_init(t_buf *buf)
{
    buf->len = 0;
}

void buf_flush(t_buf *buf)
{
    if (buf->len > 0)
    {
        write(1, buf->data, buf->len);
        buf->len = 0;
    }
}

/**
* @brief Append a known-length byte sequence to the buffer.
* Flushes whenever the buffer fills up.
*/
void buf_write(t_buf *buf, const char *src, int len)
{
    int chunk;
    int i;

    while (len > 0)
    {
        if (buf->len == BUF_SIZE)
            buf_flush(buf);
        chunk = BUF_SIZE - buf->len;
        if (chunk > len)
            chunk = len;
        i = 0;
        while (i < chunk)
        {
            buf->data[buf->len + i] = src[i];
            i++;
        }
        buf->len += chunk;
        src += chunk;
        len -= chunk;
    }
}

/**
 * @brief Append a null-terminated string.
 */
void    buf_write_str(t_buf *buf, const char *s)
{
    int len;

    len = 0;
    while (s[len])
        len++;
    buf_write(buf, s, len);
}

/**
* @brief Append a null-terminated string.
*/
void    buf_write_char(t_buf *buf, char c)
{
    if (buf->len >= BUF_SIZE)
        buf_flush(buf);
    buf->data[buf->len++] = c;
}


/**
* @brief Write an unsigned integer in decimal
* Builds digits backwards into a stack-local buffer (max 20 digits for 64-bit),
* then writes them in order.
*/
void buf_write_uint(t_buf *buf, unsigned long n)
{
    char digits[24];
    int i;

    if (n == 0)
    {
        buf_write_char(buf, '0');
        return ;
    }
    i = 0;
    while (n > 0)
    {
        digits[i++] = '0' + (n % 10);
        n /= 10;
    }
    while (i-- > 0)
        buf_write_char(buf, digits[i]);
}

/**
* @brief Compute how many decimal digits an unsigned inter takes.
* Used for column-width calculations
*/
int uint_width(unsigned long n)
{
    int w;

    if (n == 0)
        return (1);
    w = 0;
    while (n > 0)
    {
        w++;
        n /= 10;
    }
    return (w);
}

/**
 * @brief Write n copies of c. Used for column padding.
 *
 * @param buf 
 * @param c 
 * @param n 
 */
void buf_write_pad(t_buf *buf, char c, int n)
{
    while (n-- > 0)
        buf_write_char(buf, c);
}

