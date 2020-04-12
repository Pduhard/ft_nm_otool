#include "ft_nm.h"

extern void *strtab_addr;
extern off_t fsize;

void swap_sym(t_symtab *a, t_symtab *b)
{
  t_symtab tmp;

  tmp = *b;
  *b = *a;
  *a = tmp;
}

int   alph_cmp_symtab(t_symtab *a, t_symtab *b)
{
    int  cmp;
    // if (a->sym.n_un.n_strx > fsize)
    //   return (-1);
    // else if (b->sym.n_un.n_strx > fsize)
    //   return (1);
    // else
    // return (ft_strcmp(a->sym.n_un.n_strx + strtab_addr, b->sym.n_un.n_strx + strtab_addr));
    //

    cmp = ft_strcmp(a->name, b->name);
    return (cmp);
}

int   num_cmp_symtab(t_symtab *a, t_symtab *b)
{
    if (a->sym.n_value > b->sym.n_value)
      return (1);
    if (b->sym.n_value > a->sym.n_value)
      return (-1);
    return (ft_strcmp(a->name, b->name));
}

int   alph_rcmp_symtab(t_symtab *a, t_symtab *b)
{
    int  cmp;

    cmp = ft_strcmp(b->name, a->name);
    return (cmp);
}

int   num_rcmp_symtab(t_symtab *a, t_symtab *b)
{
    if (a->sym.n_value > b->sym.n_value)
      return (-1);
    if (b->sym.n_value > a->sym.n_value)
      return (1);
    return (ft_strcmp(b->name, a->name));
}

void qsort_symtab(t_pinfo *pinfo, uint32_t low, uint32_t high, int (*cmp)(t_symtab *, t_symtab *))
{
  t_symtab  *pivot;
  uint32_t  i;
  uint32_t  j;

  i = low;
  j = low;
  if (low >= high)
    return ;
  pivot = &(pinfo->symtab[high - 1]);
  while (j < high - 1)
  {
    if (pinfo->symtab[j].sym.n_un.n_strx > pinfo->fsize)
      swap_sym(&(pinfo->symtab[i++]), &(pinfo->symtab[j]));
    else if (pivot->sym.n_un.n_strx > pinfo->fsize)
      ;
    else if (ft_strcmp(pinfo->symtab[j].sym.n_un.n_strx + strtab_addr, pivot->sym.n_un.n_strx + strtab_addr))
      swap_sym(&(pinfo->symtab[i++]), &(pinfo->symtab[j]));
  //  else if (cmp(&(pinfo->symtab[j]), pivot) < 0)
    //  swap_sym(&(pinfo->symtab[i++]), &(pinfo->symtab[j]));
    j++;
  }
  (void)cmp;
  swap_sym(&(pinfo->symtab[i]), pivot);
  qsort_symtab(pinfo, low, i, cmp);
  qsort_symtab(pinfo, i + 1, high, cmp);
}

void sort_symtab(t_pinfo *pinfo)
{
  uint32_t  flags;

  flags = ((t_nm_options *)pinfo->options)->flags;
  int   (*fct)(t_symtab *, t_symtab *);

  if ((flags & OPT_N))
    fct = (flags & OPT_R) ? &num_rcmp_symtab : &num_cmp_symtab;
  else
    fct = (flags & OPT_R) ? &alph_rcmp_symtab : &alph_cmp_symtab;
  qsort(pinfo->symtab, pinfo->symid, sizeof(t_symtab), (int (*)(const void *, const void *))fct); // difference when name is identical ....
  //qsort_symtab(pinfo, 0, pinfo->symid, &alph_cmp_symtab);
  /*int j = pinfo->symid - 1;
  for (int i = 0; i < j; ++i, --j)
  {
     swap_sym(&(pinfo->symtab[i]), &(pinfo->symtab[j]));
   }*/
}
