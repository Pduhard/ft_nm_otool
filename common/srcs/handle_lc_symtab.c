#include "common.h"

// extern void *strtab_addr;

int get_sect_names(char segname[16], char sectname[16], t_pinfo *pinfo, uint8_t n_sect)
{
  if (n_sect == 0 || n_sect > pinfo->secid)
    return (0);
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
  return (1);
}

int check_section_selected(t_pinfo *pinfo, uint8_t n_sect, uint8_t n_type)
{
  char  sectname[16];
  char  segname[16];
  t_nm_options  *options;

  options = (t_nm_options *)pinfo->options;
  if ((n_type & N_TYPE) != N_SECT || !get_sect_names(segname, sectname, pinfo, n_sect))
    return (0);
  if (!ft_strcmp(options->segname, segname) && !ft_strcmp(options->sectname, sectname))
    return (1);
  return (0);
}

char  get_section_symbol(t_pinfo *pinfo, uint8_t n_sect)
{
  char  sectname[16];
  char  segname[16];
//  printf("ab %d\n", n_sect);

  if (!get_sect_names(segname, sectname, pinfo, n_sect))
    return ('S');
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

int  check_lc_symtab(struct load_command *load_command, t_pinfo *pinfo, uint32_t load_cmd_id)
{
  struct symtab_command *symtab_cmd;

  symtab_cmd = (struct symtab_command *)load_command;
  if (pinfo->get_uint32_t(load_command->cmdsize) != sizeof(struct symtab_command))
  {
    ft_fdprintf(2, "malformed object (LC_SYMTAB command %u has incorrect cmdsize)\n", load_cmd_id);
    return (0);
  }
  if (pinfo->symtab)
  {
    ft_fdprintf(2, "malformed object (more than one LC_SYMTAB command)\n");
    return (0);
  }
  if ((off_t)(pinfo->get_uint32_t(symtab_cmd->symoff) + pinfo->get_uint32_t(symtab_cmd->nsyms) * (pinfo->arch == 64 ? sizeof(struct nlist_64) : sizeof(struct nlist))) > pinfo->fsize)
  {
    ft_fdprintf(2, "truncated or malformed object (symbol table offset plus size of LC_SYMTAB in command %u extends past the end of the file)\n", load_cmd_id);
    return (0);
  }
  if ((off_t)(pinfo->get_uint32_t(symtab_cmd->stroff) + pinfo->get_uint32_t(symtab_cmd->strsize)) > pinfo->fsize)
  {
    ft_fdprintf(2, "truncated or malformed object (string table offset plus size of LC_SYMTAB in command %u extends past the end of the file)\n", load_cmd_id);
    return (0);
  }
  if ( (off_t)(pinfo->get_uint32_t(symtab_cmd->symoff) + pinfo->get_uint32_t(symtab_cmd->nsyms) * (pinfo->arch == 64 ? sizeof(struct nlist_64) : sizeof(struct nlist))) > (off_t)(pinfo->get_uint32_t(symtab_cmd->stroff)))
  {
    ft_fdprintf(2, "malformed object (string table at offset %u with a size of %u, overlaps symbol table at offset %u with a size of %zu)\n"
      , pinfo->get_uint32_t(symtab_cmd->stroff), pinfo->get_uint32_t(symtab_cmd->strsize)
      , pinfo->get_uint32_t(symtab_cmd->symoff), pinfo->get_uint32_t(symtab_cmd->nsyms) * (pinfo->arch == 64 ? sizeof(struct nlist_64) : sizeof(struct nlist)));
    return (0);
  }
  return (1);
}

void handle_lc_symtab(void *addr, t_pinfo *pinfo, void *filestart)
{
  struct symtab_command *symtab_cmd;
  void                  *symtab;
  void                  *strtab_addr;
  uint32_t              nsyms;
  char                  *symname;
  struct nlist_64       nlist;
  char                  symbol;
  char                  *indr;
  uint32_t              flags;

  // printf("%c\n", get_symbol(0, 0, 0, NULL));
  flags = ((t_nm_options *)pinfo->options)->flags;
  // struct section_64     *section;

  //printf("%zu, %zu\n", sizeof(int), sizeof(unsigned long));
  indr = NULL;
  symtab_cmd = (struct symtab_command *)addr;
  symtab = filestart + pinfo->get_uint32_t(symtab_cmd->symoff);
  strtab_addr = filestart + pinfo->get_uint32_t(symtab_cmd->stroff);
//  printf("%lld\n", (unsigned long long)strtab_addr);
  nsyms = pinfo->get_uint32_t(symtab_cmd->nsyms);
  update_symtab_size(pinfo, nsyms);
  //printf("nb syms %u symoff %x stroff %x\n", nsyms, pinfo->get_uint32_t(symtab_cmd->symoff), symtab_cmd->stroff);
  while (nsyms--)
  {
    //printf("%u\n", ((struct nlist *)(symtab))->n_un.n_strx);
    if (pinfo->arch == 32)
    {
      if (pinfo->get_uint32_t(((struct nlist *)(symtab))->n_un.n_strx) == 0)
        symname = ft_strdup("");
      else if (pinfo->get_uint32_t(((struct nlist *)(symtab))->n_un.n_strx) > pinfo->get_uint32_t(symtab_cmd->strsize))
        symname = ft_strdup("bad string index");
      // else
      //   symname = (char *)(strtab_addr + pinfo->get_uint32_t(((struct nlist *)(symtab))->n_un.n_strx));
      else if ((off_t)((void *)(strtab_addr + pinfo->get_uint32_t(((struct nlist *)(symtab))->n_un.n_strx)) - filestart) < pinfo->fsize)
        symname = ft_strndup((char *)(strtab_addr + pinfo->get_uint32_t(((struct nlist *)(symtab))->n_un.n_strx)), pinfo->fsize - (off_t)((void *)(strtab_addr + pinfo->get_uint32_t(((struct nlist *)(symtab))->n_un.n_strx)) - filestart));

      //  symname = (char *)(strtab_addr + pinfo->get_uint32_t(((struct nlist_64 *)(symtab))->n_un.n_strx));
      else
        symname = ft_strdup("bad string index");
      nlist.n_un.n_strx = pinfo->get_uint32_t(((struct nlist *)(symtab))->n_un.n_strx);
      nlist.n_desc = pinfo->get_uint16_t(((struct nlist *)(symtab))->n_desc);
      nlist.n_value = pinfo->get_uint32_t(((struct nlist *)(symtab))->n_value);
      nlist.n_type = ((struct nlist *)(symtab))->n_type;
      nlist.n_sect = ((struct nlist *)(symtab))->n_sect;
      symbol = get_symbol(nlist.n_type, nlist.n_sect, nlist.n_value, pinfo);
      if ((flags & OPT_L) && (flags & OPT_S) && check_section_selected(pinfo, nlist.n_sect, nlist.n_type))
      {
        if ((nlist.n_type & N_TYPE) == N_SECT)
        {
          if (nlist.n_value == pinfo->get_uint32_t(((struct section *)(pinfo->sectab + nlist.n_sect - 1)->secaddr)->addr))
            pinfo->section_start = 1;
        }
      }
      if ((symbol != '-' || ((flags & OPT_A) && !(flags & OPT_G)))
        && ((nlist.n_type & N_EXT) || !(flags & OPT_G))
          && (!(flags & OPT_U) || (symbol == 'U' && !(nlist.n_type & ~(N_TYPE | N_EXT))))
            && (!(flags & OPT_MAJ_U) || symbol != 'U' || (nlist.n_type & ~(N_TYPE | N_EXT)))
              && (!(flags & OPT_S) || check_section_selected(pinfo, nlist.n_sect, nlist.n_type)))
      {
        if (symbol == 'I' || symbol == 'i')
        {
          if (nlist.n_value == 0)
            indr = ft_strdup("");
          else if (nlist.n_value > (uint64_t)pinfo->get_uint32_t(symtab_cmd->strsize))
            indr = ft_strdup("bad string index");
          else
            indr = ft_strndup((char *)(strtab_addr + nlist.n_value), pinfo->fsize - (off_t)((void *)(strtab_addr + nlist.n_value) - filestart));
        }
      //maprintf("salut sym\n");

        pinfo->symtab[pinfo->symid++] = (t_symtab){nlist, symbol, symname, indr};
      }
      else
        free(symname);
    //  printf("nsect %u n_type %x n_type & N_TYPE %x %x\n", ((struct nlist *)(symtab))->n_sect, ((struct nlist *)(symtab))->n_type, ((struct nlist *)(symtab))->n_type & N_TYPE, ((struct nlist *)(symtab))->n_value);
      symtab += sizeof(struct nlist);
    }
    if (pinfo->arch == 64)
    {
    //  printf("%u\n", pinfo->get_uint32_t(((struct nlist_64 *)(symtab))->n_un.n_strx));
      if (pinfo->get_uint32_t(((struct nlist_64 *)(symtab))->n_un.n_strx) == 0)
        symname = ft_strdup("");
      else if (pinfo->get_uint32_t(((struct nlist_64 *)(symtab))->n_un.n_strx) > pinfo->get_uint32_t(symtab_cmd->strsize))
        symname = ft_strdup("bad string index");
      else if ((off_t)((void *)(strtab_addr + pinfo->get_uint32_t(((struct nlist_64 *)(symtab))->n_un.n_strx)) - filestart) < pinfo->fsize)
        symname = ft_strndup((char *)(strtab_addr + pinfo->get_uint32_t(((struct nlist_64 *)(symtab))->n_un.n_strx)), pinfo->fsize - (off_t)((void *)(strtab_addr + pinfo->get_uint32_t(((struct nlist_64 *)(symtab))->n_un.n_strx)) - filestart));
      else
        symname = ft_strdup("bad string index");
//      printf("%p %p diff %llu", symname, filestart, (unsigned long long)((unsigned long long)symname - (unsigned long long)filestart));
    //  printf("salut\n");
      nlist = *(struct nlist_64 *)(symtab);
      nlist.n_un.n_strx = pinfo->get_uint32_t(nlist.n_un.n_strx);
      nlist.n_value = pinfo->get_uint64_t(nlist.n_value);
      nlist.n_desc = pinfo->get_uint16_t(nlist.n_desc);
      symbol = get_symbol(nlist.n_type, nlist.n_sect, nlist.n_value , pinfo);
      if ((flags & OPT_L) && (flags & OPT_S) && check_section_selected(pinfo, nlist.n_sect, nlist.n_type))
      {
        if ((nlist.n_type & N_TYPE) == N_SECT)
        {
          if (nlist.n_value == pinfo->get_uint64_t(((struct section_64 *)(pinfo->sectab + nlist.n_sect - 1)->secaddr)->addr))
            pinfo->section_start = 1;
        }
      }
      if ((symbol != '-' || ((flags & OPT_A) && !(flags & OPT_G)))
        && ((nlist.n_type & N_EXT) || !(flags & OPT_G))
          && (!(flags & OPT_U) || (symbol == 'U' && !(nlist.n_type & ~(N_TYPE | N_EXT))))
            && (!(flags & OPT_MAJ_U) || symbol != 'U' || (nlist.n_type & ~(N_TYPE | N_EXT)))
              && (!(flags & OPT_S) || check_section_selected(pinfo, nlist.n_sect, nlist.n_type)))

      {
        if (symbol == 'I' || symbol == 'i')
        {
          if (nlist.n_value == 0)
            indr = ft_strdup("");
          else if (nlist.n_value > (uint64_t)pinfo->get_uint32_t(symtab_cmd->strsize))
            indr = ft_strdup("bad string index");
          else
            indr = ft_strndup((char *)(strtab_addr + nlist.n_value), pinfo->fsize - (off_t)((void *)(strtab_addr + nlist.n_value) - filestart));
        }
      //maprintf("salut sym\n");

        pinfo->symtab[pinfo->symid++] = (t_symtab){nlist, symbol, symname, indr};
      }
      else
        free(symname);
      //printf("nsect %u n_type %x n_type & N_TYPE %x %llx\n", ((struct nlist_64 *)(symtab))->n_sect, ((struct nlist_64 *)(symtab))->n_type, ((struct nlist_64 *)(symtab))->n_type & N_TYPE, ((struct nlist_64 *)(symtab))->n_value);
      symtab += sizeof(struct nlist_64);
    }
  //  printf("sym name %s\n", symname);
  }
  //printf("zefzefzef\n");
  // while (nsects--)
  // {
  //     printf("secname %s in seg %s\n", section->sectname, section->segname);
  //     section = (void *)section + sizeof(struct section_64);
  // }
  // printf("segname %s vm addr %llx nb sections %u file off %llu\n", symtab->segname, symtab->addr, symtab->nsects, symtab->fileoff);
  // (void)pinfo;
}
