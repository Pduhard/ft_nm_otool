#include "ft_nm.h"

char  get_section_symbol(t_pinfo *pinfo, uint8_t n_sect)
{
  char  sectname[16];
  //printf("ab %d\n", n_sect);
  if (pinfo->arch == 32)
    ft_memcpy(sectname, ((struct section *)(pinfo->sectab + n_sect - 1)->secaddr)->sectname, 16);
  if (pinfo->arch == 64)
    ft_memcpy(sectname, ((struct section_64 *)(pinfo->sectab + n_sect - 1)->secaddr)->sectname, 16);
  //printf("zefzefzef\n");
  if (!ft_strcmp(sectname, SECT_TEXT))
    return ('T');
  else if (!ft_strcmp(sectname, SECT_DATA))
    return ('D');
  else if (!ft_strcmp(sectname, SECT_BSS))
    return ('B');
  else
    return ('S');
}

char  get_symbol(uint8_t n_type, uint8_t n_sect, uint64_t n_value, t_pinfo *pinfo)
{
  char  sym;
//  printf("ntype %x: ", n_type);
  if ((n_type & N_STAB))
    sym = '-';
  else if ((n_type & N_TYPE) == N_UNDF)
  {
    if ((n_type & N_EXT) && n_value)
      sym = 'C';
    else
      sym = 'U';
  }
  else if ((n_type & N_TYPE) == N_ABS)
    sym = 'A';
  else if ((n_type & N_TYPE) == N_SECT)
  {
    sym = get_section_symbol(pinfo, n_sect);
    if (!(n_type & N_EXT))
      sym -= ('A' - 'a');
  }
  else //perhaps others ?????
    sym = 'U';
  return (sym);
}

uint64_t  get_sym_n_value(t_pinfo *pinfo, uint32_t i)
{
  if (pinfo->arch == ARCH_32)
    return (((struct nlist *)((pinfo->symtab + i)->symaddr))->n_value);
  if (pinfo->arch == ARCH_64)
    return (((struct nlist_64 *)((pinfo->symtab + i)->symaddr))->n_value);
  return (0);
}

uint8_t  get_sym_n_type(t_pinfo *pinfo, uint32_t i)
{
  if (pinfo->arch == ARCH_32)
    return (((struct nlist *)((pinfo->symtab + i)->symaddr))->n_type);
  if (pinfo->arch == ARCH_64)
    return (((struct nlist_64 *)((pinfo->symtab + i)->symaddr))->n_type);
  return (0);
}

uint8_t  get_sym_n_sect(t_pinfo *pinfo, uint32_t i)
{
  if (pinfo->arch == ARCH_32)
    return (((struct nlist *)((pinfo->symtab + i)->symaddr))->n_sect);
  if (pinfo->arch == ARCH_64)
    return (((struct nlist_64 *)((pinfo->symtab + i)->symaddr))->n_sect);
  return (0);
}

void  assign_symbol(t_pinfo *pinfo, uint32_t options)
{
  // t_symtab  *symtab;
  uint32_t  i;
  char      symbol;
  uint64_t  n_value;
  uint8_t   n_type;
  uint8_t   n_sect;

  i = 0;
  // symtab = pinfo->symtab;
//  printf("%d\n", pinfo->symid);
  while (i < pinfo->symid)
  {
      n_value = get_sym_n_value(pinfo, i);
      n_type = get_sym_n_type(pinfo, i);
      n_sect = get_sym_n_sect(pinfo, i);
      symbol = get_symbol(n_type, n_sect, n_value, pinfo);
    //  printf("hallo sym %c\n", symbol);
      if (symbol != '-' || (options & OPT_A))
      {
        if (symbol != 'U')
          printf(pinfo->arch == ARCH_32 ? "%08llx %c %s\n" : "%016llx %c %s\n"/* strx %u, type %hhx sect %hhu, desc %hx\n"*/, n_value,
            symbol, (pinfo->symtab + i)->name);//, ((struct nlist_64 *)((pinfo->symtab + i)->symaddr))->n_un.n_strx, ((struct nlist_64 *)((pinfo->symtab + i)->symaddr))->n_type, ((struct nlist_64 *)((pinfo->symtab + i)->symaddr))->n_sect, ((struct nlist_64 *)((pinfo->symtab + i)->symaddr))->n_desc);
         else
           printf(pinfo->arch == ARCH_32 ? "%8c %c %s\n" : "%16c %c %s\n", ' ', symbol, (pinfo->symtab + i)->name);
      }
    i++;
  }
}
