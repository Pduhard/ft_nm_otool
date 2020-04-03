#include "ft_nm.h"

uint32_t  reverse_uint32_t(uint32_t nb)
{
  return ((nb >> 24 & 0xff) | (nb >> 8 & 0xff00) | (nb << 8 & 0xff0000) | (nb << 24 & 0xff000000));
}

uint32_t  same_uint32_t(uint32_t nb)
{
  return (nb);
}
