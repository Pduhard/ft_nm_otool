#include "ft_nm.h"

uint32_t  reverse_uint32_t(uint32_t nb)
{
  return ((nb >> 24 & 0xff) | (nb >> 8 & 0xff00) | (nb << 8 & 0xff0000) | (nb << 24 & 0xff000000));
}

uint32_t  same_uint32_t(uint32_t nb)
{
  return (nb);
}

uint64_t  reverse_uint64_t(uint64_t nb)
{
  return ((nb >> 56 & 0xff) | (nb >> 40 & 0xff00) | (nb >> 24 & 0xff0000) | (nb >> 8 & 0xff000000)
    | (nb << 8 & 0xff00000000) | (nb << 24 & 0xff0000000000) | (nb << 40 & 0xff000000000000) | (nb << 56 & 0xff00000000000000));
}

uint64_t  same_uint64_t(uint64_t nb)
{
  return (nb);
}
