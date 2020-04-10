#include "ft_nm.h"

char  get_section_symbol(t_pinfo *pinfo, uint8_t n_sect)
{
  char  sectname[16];
  char  segname[16];
//  printf("ab %d\n", n_sect);
  if (n_sect == 0 || n_sect > pinfo->secid)
    return ('S');
  if (pinfo->arch == 32)
  {
    ft_memcpy(sectname, ((struct section *)(pinfo->sectab + n_sect - 1)->secaddr)->sectname, 16);
    ft_memcpy(segname, ((struct section *)(pinfo->sectab + n_sect - 1)->secaddr)->segname, 16);
  }
  if (pinfo->arch == 64)
  {
    ft_memcpy(sectname, ((struct section_64 *)(pinfo->sectab + n_sect - 1)->secaddr)->sectname, 16);
    ft_memcpy(segname, ((struct section_64 *)(pinfo->sectab + n_sect - 1)->secaddr)->segname, 16);
  }
//  printf("zefzefzef\n");
  if (!ft_strcmp(sectname, SECT_TEXT) && !ft_strcmp(segname, SEG_TEXT))
    return ('T');
  else if (!ft_strcmp(sectname, SECT_DATA) && !ft_strcmp(segname, SEG_DATA))
    return ('D');
  else if (!ft_strcmp(sectname, SECT_BSS) && !ft_strcmp(segname, SEG_DATA))
    return ('B');
  else
    return ('S');
}

char  get_symbol(uint8_t n_type, uint8_t n_sect, uint64_t n_value, t_pinfo *pinfo)
{
  char  sym;
  uint8_t type_m;

  type_m = n_type & N_TYPE;
  //printf("ntype %2x\n", n_type);
  if ((n_type & N_STAB))
    sym = '-';
  else if (type_m == N_UNDF)
    sym = n_value ? 'C' : 'U';
  else if (type_m == N_ABS)
    sym = 'A';
  else if (type_m == N_SECT)
    sym = get_section_symbol(pinfo, n_sect);
  else if (type_m == N_INDR)
    sym = 'I';
  else if (type_m == N_PBUD)
    sym = 'U';
  else
    sym = '?';
  if (sym != '?' && sym != '-' && !(n_type & N_EXT))
    sym -= ('A' - 'a');
  return (sym);
}
/*
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
*/

char  *get_stab_name(uint8_t n_type)
{
  char *undef;

  undef = NULL;
  if (n_type == N_GSYM)
    return (ft_strdup("GSYM"));
  if (n_type == N_FNAME)
    return (ft_strdup("FNAME"));
  if (n_type == N_FUN)
    return (ft_strdup("FUN"));
  if (n_type == N_STSYM)
    return (ft_strdup("STSYM"));
  if (n_type == N_LCSYM)
    return (ft_strdup("LCSYM"));
  if (n_type == N_BNSYM)
    return (ft_strdup("BNSYM"));
  if (n_type == N_OPT)
    return (ft_strdup("OPT"));
  if (n_type == N_RSYM)
    return (ft_strdup("RSYM"));
  if (n_type == N_SLINE)
    return (ft_strdup("SLINE"));
  if (n_type == N_ENSYM)
    return (ft_strdup("ENSYM"));
  if (n_type == N_SSYM)
    return (ft_strdup("SSYM"));
  if (n_type == N_SO)
    return (ft_strdup("SO"));
  if (n_type == N_OSO)
    return (ft_strdup("OSO"));
  if (n_type == N_LSYM)
    return (ft_strdup("LSYM"));
  if (n_type == N_BINCL)
    return (ft_strdup("BINCL"));
  if (n_type == N_SOL)
    return (ft_strdup("SOL"));
  if (n_type == N_PARAMS)
    return (ft_strdup("PARAM"));
  if (n_type == N_VERSION)
    return (ft_strdup("VERS"));
  if (n_type == N_OLEVEL)
    return (ft_strdup("OLEV"));
  if (n_type == N_PSYM)
    return (ft_strdup("PSYM"));
  if (n_type == N_EINCL)
    return (ft_strdup("EINCL"));
  if (n_type == N_ENTRY)
    return (ft_strdup("ENTRY"));
  if (n_type == N_LBRAC)
    return (ft_strdup("LBRAC"));
  if (n_type == N_EXCL)
    return (ft_strdup("EXCL"));
  if (n_type == N_RBRAC)
    return (ft_strdup("RBRAC"));
  if (n_type == N_BCOMM)
    return (ft_strdup("BCOMM"));
  if (n_type == N_ECOMM)
    return (ft_strdup("ECOMM"));
  if (n_type == N_ECOML)
    return (ft_strdup("ECOML"));
  if (n_type == N_LENG)
    return (ft_strdup("LENG"));
  if (n_type == N_PC)
    return (ft_strdup("PC"));
  ft_getf(&undef, "%02hhx", n_type);
  return (undef);
}

void  assign_symbol(t_pinfo *pinfo)
{
  // t_symtab  *symtab;
  t_nm_options *options;
  uint32_t  i;
  char      *tmp;

  options = (t_nm_options *)pinfo->options;
  // char      symbol;
  // uint64_t  n_value;
  // uint8_t   n_type;
  // uint8_t   n_sect;

  i = 0;
  // symtab = pinfo->symtab;
//  printf("%d\n", pinfo->symid);
  while (i < pinfo->symid)
  {
      // n_value = get_sym_n_value(pinfo, i);
      // n_type = get_sym_n_type(pinfo, i);
      // n_sect = get_sym_n_sect(pinfo, i);
      // symbol = get_symbol(n_type, n_sect, n_value, pinfo);
      // printf("hallo %x %x %x\n", REFERENCE_TYPE, (uint32_t)(pinfo->symtab + i)->sym.n_desc & REFERENCE_TYPE, (uint32_t)(pinfo->symtab + i)->sym.n_desc & ~REFERENCE_TYPE);
    //  if ((pinfo->symtab + i)->symbol != '-' || (options->flags & OPT_A))
    //  {
       //  printf("%c\n", (pinfo->symtab + i)->symbol);
       // printf("%s\n", (pinfo->symtab + i)->name);
        if ((pinfo->symtab + i)->symbol == '-')
        {

        printf(pinfo->arch == ARCH_32 ? "%08llx %c %02x %04x %5.5s %s\n" : "%016llx %c %02x %04x %5.5s %s\n", (pinfo->symtab + i)->sym.n_value,
          (pinfo->symtab + i)->symbol, (uint32_t)(pinfo->symtab + i)->sym.n_sect & 0xff, (unsigned int)(pinfo->symtab + i)->sym.n_desc & 0xffff, (tmp = get_stab_name((pinfo->symtab + i)->sym.n_type)),  (pinfo->symtab + i)->name);
          //, (pinfo->symtab + i)->sym.n_un.n_strx, (pinfo->symtab + i)->sym.n_type, (pinfo->symtab + i)->sym.n_sect, (pinfo->symtab + i)->sym.n_desc);
          if (tmp)
            free(tmp);
        }
        else if ((pinfo->symtab + i)->symbol == 'I' || (pinfo->symtab + i)->symbol == 'i')
          printf(pinfo->arch == ARCH_32 ? "%8c %c %s (indirect for %s)\n" : "%16c %c %s (indirect for %s)\n", ' ', (pinfo->symtab + i)->symbol, (pinfo->symtab + i)->name, (pinfo->symtab + i)->indr);
        else if ((pinfo->symtab + i)->symbol != 'U' && (pinfo->symtab + i)->symbol != 'u')
          printf(pinfo->arch == ARCH_32 ? "%08llx %c %s\n" : "%016llx %c %s\n"/* strx %u, type %hhx sect %hhu, desc %hx\n"*/, (pinfo->symtab + i)->sym.n_value,
            (pinfo->symtab + i)->symbol, (pinfo->symtab + i)->name);//, ((struct nlist_64 *)((pinfo->symtab + i)->symaddr))->n_un.n_strx, ((struct nlist_64 *)((pinfo->symtab + i)->symaddr))->n_type, ((struct nlist_64 *)((pinfo->symtab + i)->symaddr))->n_sect, ((struct nlist_64 *)((pinfo->symtab + i)->symaddr))->n_desc);
        else
           printf(pinfo->arch == ARCH_32 ? "%8c %c %s\n" : "%16c %c %s\n", ' ', (pinfo->symtab + i)->symbol, (pinfo->symtab + i)->name);
    //  }
    i++;
  }
}
