#include "ft_nm.h"


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


void  human_readable_print_symbol(t_pinfo *pinfo, t_symtab *symbol)
{
  char  segname[16];
  char  sectname[16];

  if (symbol->symbol == 'U' || symbol->symbol == 'u' || symbol->symbol == 'I' || symbol->symbol == 'i')
    printf(pinfo->arch == ARCH_32 ? "         " : "                 ");
  else
    printf(pinfo->arch == ARCH_32 ? "%08llx " : "%0816llx ", symbol->sym.n_value);
  if ((symbol->sym.n_type & N_TYPE) == N_SECT)
  {
    if (get_sect_names(segname, sectname, pinfo, symbol->sym.n_sect))
      printf("(%.16s,%.16s) ", segname, sectname);
    else
      printf("(,) ");
  }
  else if ((symbol->sym.n_type & N_TYPE) == N_UNDF)
    printf(symbol->sym.n_value ? "(common) " : "(undefined) ");
  else if ((symbol->sym.n_type & N_TYPE) == N_ABS)
    printf("(absolute) ");
  else if ((symbol->sym.n_type & N_TYPE) == N_INDR)
    printf("(indirect) ");
  else
    printf("(undefined) ");
  if ((symbol->sym.n_type & N_EXT))
    printf("external %s\n", symbol->name);
  else
    printf("non-external %s\n", symbol->name);
}

void  print_xval_symbol(t_pinfo *pinfo, t_symtab *symtab)
{
  if (symtab->symbol == 'I' || symtab->symbol == 'i')
    printf(pinfo->arch == ARCH_32 ? "%08llx %02hhx %02hhx %04hx %08x %s (indirect for %08llx %s)\n" : "%016llx %02hhx %02hhx %04hx %08x %s (indirect for %016llx %s)\n", symtab->sym.n_value, symtab->sym.n_type, symtab->sym.n_sect, symtab->sym.n_desc, symtab->sym.n_un.n_strx, (off_t)symtab->sym.n_un.n_strx > pinfo->fsize ? "(bad string index)" : symtab->name, symtab->sym.n_value, (off_t)symtab->sym.n_un.n_strx > pinfo->fsize ? "(bad string index)" : symtab->indr);
  else
    printf(pinfo->arch == ARCH_32 ? "%08llx %02hhx %02hhx %04hx %08x %s\n" : "%016llx %02hhx %02hhx %04hx %08x %s\n", symtab->sym.n_value, symtab->sym.n_type, symtab->sym.n_sect, symtab->sym.n_desc, symtab->sym.n_un.n_strx, (off_t)symtab->sym.n_un.n_strx > pinfo->fsize ? "(bad string index)" : symtab->name);
}

void  portable_output_format(t_pinfo *pinfo, t_symtab *symtab)
{
  // uint32_t flags;
  t_nm_options *options;

  options = (t_nm_options *)pinfo->options;

  // flags = options->flags;
  if ((options->flags & OPT_T) && options->format == F_OCT)
    printf("%s %c %llo 0\n", symtab->name, symtab->symbol, symtab->sym.n_value);
  else if ((options->flags & OPT_T) && options->format == F_DEC)
    printf("%s %c %lld 0\n", symtab->name, symtab->symbol, symtab->sym.n_value);
  else
    printf("%s %c %llx 0\n", symtab->name, symtab->symbol, symtab->sym.n_value);
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
        if (((options->flags & OPT_O) && !(options->flags & OPT_MAJ_P)) || (options->flags & OPT_MAJ_A))
        {
          if (pinfo->fat_arch_from)
            printf((options->flags & OPT_MAJ_P) ? "(for architecture %s): " : "(for architecture %s):", pinfo->fat_arch_from);
          printf("%s", pinfo->file_name);
          if (pinfo->ar_from)
            printf((options->flags & OPT_MAJ_P) ? "[%s]: " : ":%s: ", pinfo->ar_from);
          else
            printf(": ");
        }
        // if ((options->flags & OPT_M))
        //   human_readable_print_symbol(pinfo, pinfo->symtab + i);
        if ((options->flags & OPT_X))
          print_xval_symbol(pinfo, pinfo->symtab + i);
        else if ((options->flags & OPT_MAJ_P))
          portable_output_format(pinfo, pinfo->symtab + i);
        else if ((pinfo->symtab + i)->symbol == '-')
        {

        printf(pinfo->arch == ARCH_32 ? "%08llx %c %02x %04x %5.5s %s\n" : "%016llx %c %02x %04x %5.5s %s\n", (pinfo->symtab + i)->sym.n_value,
          (pinfo->symtab + i)->symbol, (uint32_t)(pinfo->symtab + i)->sym.n_sect & 0xff, (unsigned int)(pinfo->symtab + i)->sym.n_desc & 0xffff, (tmp = get_stab_name((pinfo->symtab + i)->sym.n_type)),  (pinfo->symtab + i)->name);
          //, (pinfo->symtab + i)->sym.n_un.n_strx, (pinfo->symtab + i)->sym.n_type, (pinfo->symtab + i)->sym.n_sect, (pinfo->symtab + i)->sym.n_desc);
          if (tmp)
            free(tmp);
        }
        else if ((options->flags & (OPT_U | OPT_J)))
          printf("%s\n", (pinfo->symtab + i)->name);
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
