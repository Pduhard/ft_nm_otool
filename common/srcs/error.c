#include "common.h"

// void    throw_error()
// {
//
// }
//
void    ft_throw_error(char *message, ...)
{
  va_list	ap;

  va_start(ap, message);
  ft_fdvprintf(2, message , ap);
  va_end(ap);
}

int     ft_return_error(int ret, char *message, ...)
{
  va_list	ap;

  va_start(ap, message);
  ft_fdvprintf(2, message , ap);
  va_end(ap);
  return (ret);
}

int     ft_memalloc_error(int ret, size_t size, char *prog_name)
{
  ft_fdprintf(2, "internal error: %s: too big allocation (try to allocate %zu bytes)"
    , prog_name, size);
  return (ret);
}

int     ft_usage_error(int ret, char *usage, char *prog_name)
{
  ft_fdprintf(2, "Usage: %s %s\n", prog_name, usage);
  return (ret);
}
