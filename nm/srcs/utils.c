#include "ft_nm.h"

void update_sectab_size(t_pinfo *pinfo, uint32_t addsize)
{
  t_sectab  *sectab;
  uint32_t  i;

//  printf("secid %d addsize%d\n", pinfo->secid, addsize);
  if (!(sectab = ft_memalloc(sizeof(t_sectab) * (pinfo->secid + addsize))))
    exit(EXIT_FAILURE);
  i = -1;
//printf("i = %d\n", i);
  while (++i < pinfo->secid)
    sectab[i] = pinfo->sectab[i];
  //printf("i = %d\n", i);

  if (i)
    free(pinfo->sectab);
  pinfo->sectab = sectab;
}

void update_symtab_size(t_pinfo *pinfo, uint32_t addsize)
{
  t_symtab  *symtab;
  uint32_t  i;

  if (!(symtab = ft_memalloc(sizeof(t_symtab) * (pinfo->symid + addsize))))
    exit(EXIT_FAILURE);
  i = -1;
  while (++i < pinfo->symid)
    symtab[i] = pinfo->symtab[i];
//  printf("i = %d\n", i);
  if (i)
    free(pinfo->symtab);
  pinfo->symtab = symtab;
}
