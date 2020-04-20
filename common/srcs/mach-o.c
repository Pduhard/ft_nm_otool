#include "common.h"

int check_arch_in_file(cpu_type_t cputype, cpu_subtype_t cpusubtype, t_pinfo *pinfo, char *arch_name)
{
  int check;
  const NXArchInfo  *arch_info;
  const NXArchInfo  *all_info;
  const NXArchInfo  *family;
  // int family;


  check = 0;
  // printf("%d\n", family);
  if (pinfo->file_type == MH_FILE)
  {
    arch_info = NXGetArchInfoFromName(arch_name);
    if (!((t_nm_options *)pinfo->options)->arch_flags[1])
    {
     if (!(all_info = NXGetAllArchInfos()))
       return (0);
     family = NULL;
     while (all_info->name)
     {
       if (all_info->cputype == arch_info->cputype)
       {
         // printf("%x %x ?\n", (all_info->cpusubtype & ~CPU_SUBTYPE_MASK), (arch_info->cpusubtype & ~CPU_SUBTYPE_MASK));
         if ((all_info->cpusubtype & ~CPU_SUBTYPE_MASK) == (arch_info->cpusubtype & ~CPU_SUBTYPE_MASK))
           family = all_info;
         break ;
       }
       ++all_info;
     }
   }
     // printf("%x %x == %x %x\n", (uint32_t)arch_info->cputype, (uint32_t)arch_info->cpusubtype, pinfo->get_uint32_t(*((uint32_t *)(mfile + sizeof(uint32_t)))), pinfo->get_uint32_t(*((uint32_t *)(mfile + sizeof(uint32_t) * 2))));

    if (arch_info->cputype != cputype
    || (((uint32_t)arch_info->cpusubtype & ~CPU_SUBTYPE_MASK) != (cpusubtype & ~CPU_SUBTYPE_MASK) && !family))
      return (0);
    else
      return (1);
  }
  return (1);
}

int       check_archs_in_file(void *mfile, t_pinfo *pinfo)
{
    t_nm_options *options;
    uint32_t  i;
    int       check;
    int       ar_check;

    // printf("???????\n");/
    check = 0;
    i = 0;
    options = (t_nm_options *)pinfo->options;
    if (!(options->flags & OPT_ARCH) || !ft_strcmp(options->arch_flags[0], "all"))
      return (1);
    while (options->arch_flags[i])
    {
      if (!(ar_check = check_arch_in_file(pinfo->get_uint32_t(*((uint32_t *)(mfile + sizeof(uint32_t)))), pinfo->get_uint32_t(*((uint32_t *)(mfile + sizeof(uint32_t) * 2))), pinfo, options->arch_flags[i])))
        ft_fdprintf(2, "file: %s does not contain architecture: %s\n", pinfo->file_name, options->arch_flags[i]);
      check |= ar_check;
      // else if (pinfo->file_type == ARCHIVE_FILE)
        // check = 1;
      i++;
    }
    return (check);
}

uint32_t  get_number_load_command(void *mfile, t_pinfo *pinfo)
{
  if (pinfo->arch == 64)
    return (pinfo->get_uint32_t(((struct mach_header_64 *)mfile)->ncmds));
  else
    return (pinfo->get_uint32_t(((struct mach_header *)mfile)->ncmds));
    // return (pinfo->get_uint32_t((*(struct mach_header **)(mfile))->ncmds));
  return (0);
}
//
// int   check_mach_header(void *mfile, t_pinfo *pinfo, uint32_t *sizeofcmds)
// {
//     struct mach_header    *hd;
//     struct mach_header_64 *hd_64;
//
//     if (pinfo->arch == 64)
//     {
//       hd_64 = (struct mach_header_64 *)mfile;
//       *sizeofcmds = pinfo->get_uint32_t(hd_64->sizeofcmds);
//       // printf("%u == %u ?\n", pinfo->get_uint32_t(hd_64->sizeofcmds), (uint32_t)pinfo->fsize);
//       //if (pinfo->fsize < )
//     }
//     else if (pinfo->arch == 32)
//     {
//       hd = (struct mach_header *)mfile;
//       *sizeofcmds = pinfo->get_uint32_t(hd->sizeofcmds);
//       // printf("%u == %u ?\n", pinfo->get_uint32_t(hd->sizeofcmds), (uint32_t)pinfo->fsize);
//     }
//     else
//       return (0);
//     return (1);
// }

int   check_macho_file(void *mfile, t_pinfo *pinfo)
{
  uint32_t            ncmds;
  struct load_command *load_c;
  uint32_t            i;
  uint32_t            sizeofcmds;
  uint32_t            final_size;

  i = 0;

  if (!pinfo->fat_arch_from && !pinfo->ar_from && !check_archs_in_file(mfile, pinfo))
    return (0);
  if (pinfo->arch == 64)
    sizeofcmds = pinfo->get_uint32_t(((struct mach_header_64 *)mfile)->sizeofcmds);
  else if (pinfo->arch == 32)
    sizeofcmds = pinfo->get_uint32_t(((struct mach_header *)mfile)->sizeofcmds);
  else
    return (0);
  ncmds = get_number_load_command(mfile, pinfo);
  final_size = 0;
  load_c = (struct load_command *)(mfile + (pinfo->arch == 64 ? sizeof(struct mach_header_64) : sizeof(struct mach_header)));
  while (i < ncmds)
  {
    // printf("load_command id %x cmd size %u %u/%u\n", load_c->cmd, load_c->cmdsize, i, ncmds);
    // if ((off_t)((void *)load_c - mfile) > pinfo->fsize)
    // {
    //   ft_fdprintf(2, "truncated or malformed object (load commands extend past the end of the file)\n");
    //   return (0);
    // }

    if (!pinfo->get_uint32_t(load_c->cmdsize))
    {
      ft_fdprintf(2, "malformed object (load command %u cmdsize is zero)\n", i);
      return (0);
    }
    if (!check_load_command(load_c, pinfo, i))
     return (0);
    final_size += pinfo->get_uint32_t(load_c->cmdsize);
    load_c = (void *)load_c + pinfo->get_uint32_t(load_c->cmdsize);

    i++;
  }
  if (final_size != sizeofcmds)
  {
    ft_fdprintf(2, "malformed object (inconsistent sizeofcmds field in mach header)\n");
    return (0);
  }
  // printf("e\n")m;
  return (1);
}

// void  create_section_start(t_pinfo *pinfo)
// {
//     t_symtab  sect_start;
//     t_symtab  *new_tab;
//     // uint32_
//     t_nm_options  *opt;
//     uint8_t   n_sect;
//     // uint64_t  n_value;
//     uint32_t  i;
//
//     opt = (t_nm_options *)pinfo->options;
//     i = 0;
//     n_sect = 0;
//     while (i < pinfo->secid)
//     {
//       if (!ft_strcmp(opt->segname, ((struct section*)(pinfo->sectab + i)->secaddr)->segname)
//         && !ft_strcmp(opt->sectname, ((struct section*)(pinfo->sectab + i)->secaddr)->sectname))
//         n_sect = i + 1;
//       i++;
//     }
//     // printf("%d\n", n_sect);
//     if (!n_sect)
//       return ;
//     if (pinfo->arch == ARCH_32 && !pinfo->get_uint32_t(((struct section *)(pinfo->sectab + n_sect - 1)->secaddr)->size))
//       return ;
//     if (pinfo->arch == ARCH_64 && !pinfo->get_uint64_t(((struct section_64 *)(pinfo->sectab + n_sect - 1)->secaddr)->size))
//         return ;
//     if (!(new_tab = ft_memalloc(sizeof(t_symtab) * (pinfo->symid + 2))))
//       return ;
//     ft_memcpy(new_tab, pinfo->symtab, sizeof(t_symtab) * pinfo->symid);
//     ft_bzero(&sect_start, sizeof(t_symtab));
//     sect_start.name = ft_strdup(".section_start");
//     sect_start.sym.n_type = N_SECT;
//     sect_start.sym.n_sect = n_sect;
//     sect_start.sym.n_value = pinfo->arch == ARCH_32 ? pinfo->get_uint32_t(((struct section *)(pinfo->sectab + n_sect - 1)->secaddr)->addr) : pinfo->get_uint64_t(((struct section_64 *)(pinfo->sectab + n_sect - 1)->secaddr)->addr);
//     sect_start.symbol = get_symbol(N_SECT, n_sect, sect_start.sym.n_value, pinfo);
//     //sect_start.symbol = 'd';
//     new_tab[pinfo->symid++] = sect_start;
//     free(pinfo->symtab);
//     pinfo->symtab = new_tab;
// }

void  handle_macho_file(void **mfile, t_pinfo *pinfo, uint32_t display)
{
  struct load_command *load_c;
  uint32_t            ncmds;
  // void                *filestart;

  if (pinfo->bin == BIN_NM && !check_macho_file(*mfile, pinfo))
    return ;

  ncmds = get_number_load_command(*mfile, pinfo);
  // printf("%lld\n", pinfo->fsize);
  //mprintf("avant loadc\n");
  load_c = (struct load_command *)(*mfile + (pinfo->arch == 64 ? sizeof(struct mach_header_64) : sizeof(struct mach_header)));
  while (ncmds--)
  {
//    printf("load_command id %x cmd size %u\n", load_c->cmd, load_c->cmdsize);
    handle_load_command(load_c, pinfo, *mfile);
    load_c = (void *)load_c + pinfo->get_uint32_t(load_c->cmdsize);
  }
  // if (pinfo->bin == BIN_NM)
  // {
    // printf("azdazd %p\n", pinfo->print);
    pinfo->print(pinfo, display);
  //   if (!(pinfo->symtab))
  //     ft_fdprintf(2, "warning: %s: no name list\n", pinfo->file_name);
  //   else if (display == DISPLAY_INFO_ON && !(((t_nm_options *)pinfo->options)->flags & OPT_O))
  //     printf("\n%s:\n", pinfo->file_name);
  //
  // //  printf("sort avant\n");
  //   if ((((t_nm_options *)pinfo->options)->flags & OPT_L) && (((t_nm_options *)pinfo->options)->flags & OPT_S) && !pinfo->section_start)
  //   {
  //     create_section_start(pinfo);
  //     // t_symtab *new_tab;
  //     //
  //     // if ((new_tab = ft_memalloc(sizeof(t_symtab) * (pinfo->symid + 2))))
  //     // {
  //     //   ft_memcpy(new_tab, pinfo->symtab, sizeof(t_symtab) * pinfo->symid);
  //     //   new_tab[pinfo->symid].name = ft_strdup(".section_start");
  //     //   new_tab[pinfo->symid].sym.n_type = N_SECT;
  //     //   new_tab[pinfo->symid].sym.n_sect = 1;
  //     //   new_tab[pinfo->symid].sym.n_value = 0xffffff;
  //     //
  //     //   new_tab[pinfo->symid].symbol = 'c';
  //     //   pinfo->symid++;
  //     //   free(pinfo->symtab);
  //     //   pinfo->symtab = new_tab;
  //     // }
  //   }
  //   if (!(((t_nm_options *)pinfo->options)->flags & OPT_P))
  //     sort_symtab(pinfo);
  //   //printf("sort apresm\n");
  //   assign_symbol(pinfo);
  // }
  // else
  //   printf("TROP BIEN\n");
}
