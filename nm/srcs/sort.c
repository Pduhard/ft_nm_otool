#include "ft_nm.h"

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

    cmp = strcmp(a->name, b->name);
  /*  if (!cmp)
    {
        if (pinfo->arch == 64)
          return (((struct nlist_64 *)(a.symaddr))->n_value - ((struct nlist_64 *)(b.symaddr))->n_value);
        else
          return (((struct nlist *)(b.symaddr))->n_value - ((struct nlist *)(a.symaddr))->n_value);
    }`*/
    // (void)pinfo;
    return (cmp);
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
    if (cmp(&(pinfo->symtab[j]), pivot) <= 0)
      swap_sym(&(pinfo->symtab[i++]), &(pinfo->symtab[j]));
    j++;
  }
  swap_sym(&(pinfo->symtab[i]), pivot);
  qsort_symtab(pinfo, low, i, cmp);
  qsort_symtab(pinfo, i + 1, high, cmp);
}

void sort_symtab(t_pinfo *pinfo)
{
    //qsort(pinfo->symtab, pinfo->symid, sizeof(t_symtab),
		  //(int (*)(const void *, const void *))alph_cmp_symtab);
  qsort_symtab(pinfo, 0, pinfo->symid, &alph_cmp_symtab);
  /*int j = pinfo->symid - 1;
  for (int i = 0; i < j; ++i, --j)
  {
     swap_sym(&(pinfo->symtab[i]), &(pinfo->symtab[j]));
   }*/
}
