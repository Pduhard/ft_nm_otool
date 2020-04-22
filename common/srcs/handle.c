#include "common.h"


int       check_arch_flags(t_option *opt)
{
  int all;
  int i;
  const NXArchInfo  *archinfo;

  all = 0;
  i = 0;
  while (opt->arch_flags[i])
  {
    if (!ft_strcmp(opt->arch_flags[i], "all"))
      all = 1;
    else if (!(archinfo = NXGetArchInfoFromName(opt->arch_flags[i])))
      return (0);
    i++;
  }
  if (all)
  {
    free(opt->arch_flags);
    if (!(opt->arch_flags = ft_memalloc(sizeof(char *) * 2)))
      return (0);
    opt->arch_flags[0] = ft_strdup("all");
  }
  return (1);
}
uint32_t           update_str_tab(char ***tab_addr, char *new, char *prog_name)
{
  uint32_t  i;
  char      **tab;
  char      **new_tab;

  tab = *tab_addr;
  i = 0;
  while (tab && tab[i])
    i++;
  if (!(new_tab = ft_memalloc(sizeof(char *) * (i + 2))))
    return (ft_memalloc_error(0, sizeof(char *) * (i + 2), prog_name));
  ft_memcpy(new_tab, tab, sizeof(char *) * i);
  if (!(new_tab[i] = ft_strdup(new)))
  {
    free(new_tab);
    return (ft_memalloc_error(0, ft_strlen(new) + 1, prog_name));
  }
  if (tab)
    free(tab);
  *tab_addr = new_tab;
  return (1);
}

uint32_t       update_arch_flags(char ***arch_flags_addr, char *new_flag, char *prog_name, uint32_t ret)
{
    if (!new_flag)
      return (ft_return_error(OPT_ERROR, "error: %s: missing argument(s) to -arch option\n", prog_name));
    if (update_str_tab(arch_flags_addr, new_flag, prog_name))
      return (ret);
    return (OPT_ERROR);
}

uint32_t       update_format(e_format *format, char *new_format, char *prog_name)
{
    if (!new_format)
      return (ft_return_error(OPT_ERROR, "error: %s: missing argument to -t option\n", prog_name));
    if (!ft_strcmp(new_format, "d"))
      *format = F_DEC;
    else if (!ft_strcmp(new_format, "o"))
      *format = F_OCT;
    else if (!ft_strcmp(new_format, "x"))
      *format = F_HEX;
    else
      return (ft_return_error(OPT_ERROR, "error: %s: invalid argument to option: -t %s\n", prog_name, new_format));
    return (OPT_NM_T);
}

char  **parse_arguments(char **argv, t_option *opt_struct,
        uint32_t (*check_valid_flag)(char **, uint32_t *, t_option *), uint32_t *flags)
{
  uint32_t  i;
  uint32_t  opt_flag;
  char      **files;

  files = NULL;
  i = 1;
  while (argv[i])
  {
    if (argv[i][0] == '-' && !(*flags & OPT_END))
    {
      if ((opt_flag = check_valid_flag(argv, &i, opt_struct)) == OPT_ERROR)
        return (NULL);
      *flags |= opt_flag;
    }
    else if (!update_str_tab(&files, argv[i], argv[0]))
      return (NULL);
    if (argv[i])
      i++;
  }
  if ((*flags & NM_DEFAULT_INPUT) && files == NULL && !update_str_tab(&files, "a.out", argv[0]))
    return (NULL);
  return (files);
}

void *mapp_file(char *file_name, char *bin_name, off_t *file_size, e_bin bin)
{
  int         fd;
  struct stat stat_buf;
  void        *mapped_file;

  mapped_file = NULL;
  if ((fd = open(file_name, O_RDONLY)) < 0 || fstat(fd, &stat_buf) < 0)
  {
    ft_fdprintf(2, "error: %s: can't open file: %s\n", bin_name, file_name);
    return (NULL);
  }
  *file_size = stat_buf.st_size;
  if (!*file_size && bin == BIN_OTOOL)
    printf("%s: is not an object file\n", file_name);
  else if (*file_size && (mapped_file = mmap(NULL, stat_buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
  {
    ft_fdprintf(2, "error: %s: can't map file: %s\n", bin_name, file_name);
    return (NULL);
  }
  close(fd);
  return (mapped_file);
}

const NXArchInfo  *get_local_arch_info()
{
    const NXArchInfo *ainfo;

    ainfo = NXGetLocalArchInfo();
#if __LP64__
    if (ainfo->cputype == CPU_TYPE_I386)
      return (NXGetArchInfoFromName("x86_64"));
#endif
  return (ainfo);
}

t_pinfo      get_parse_info(void *mfile, e_bin bin)
{
  uint32_t  magic;
  struct mach_header  *hd;
  char      *magic_str;

  hd = (struct mach_header *)mfile;
  magic = hd->magic;
  magic_str = (char *)mfile;
//  printf("magic: %0x\n", magic);
  if (magic == MH_MAGIC)
    return ((t_pinfo){&same_uint16_t, &same_uint32_t, &same_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 0, 32, bin, NULL, MH_FILE, NULL, 0, NULL, NULL, 0, mfile, hd->cputype, hd->cpusubtype, NULL});
  if (magic == MH_CIGAM)
    return ((t_pinfo){&reverse_uint16_t, &reverse_uint32_t, &reverse_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 1, 32, bin, NULL, MH_FILE, NULL, 0, NULL, NULL, 0, mfile, reverse_uint32_t(hd->cputype), reverse_uint32_t(hd->cpusubtype), NULL});
  if (magic == MH_MAGIC_64)
    return ((t_pinfo){&same_uint16_t, &same_uint32_t, &same_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 0, 64, bin, NULL, MH_FILE, NULL, 0, NULL, NULL, 0, mfile, hd->cputype, hd->cpusubtype, NULL});
  if (magic == MH_CIGAM_64)
    return ((t_pinfo){&reverse_uint16_t, &reverse_uint32_t, &reverse_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 1, 64, bin, NULL, MH_FILE, NULL, 0, NULL, NULL, 0, mfile, reverse_uint32_t(hd->cputype), reverse_uint32_t(hd->cpusubtype), NULL});
  if (magic == FAT_MAGIC)
    return ((t_pinfo){&same_uint16_t, &same_uint32_t, &same_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 0, 64, bin, NULL, FAT_FILE, NULL, 0, NULL, NULL, 0, mfile, 0, 0, NULL});
  if (magic == FAT_CIGAM)
    return ((t_pinfo){&reverse_uint16_t, &reverse_uint32_t, &reverse_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 1, 64, bin, NULL, FAT_FILE, NULL, 0, NULL, NULL, 0, mfile, 0, 0, NULL});
  if (!ft_strncmp(magic_str, ARMAG, SARMAG))
    return ((t_pinfo){&same_uint16_t, &same_uint32_t, &same_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 1, 64, bin, NULL, ARCHIVE_FILE, NULL, 0, NULL, NULL, 0, mfile, 0, 0, NULL});
  return ((t_pinfo){NULL, NULL, NULL, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 0, 0, bin, NULL, UNKNOWN_FILE, NULL, 0, NULL, NULL, 0, mfile, 0, 0, NULL});
}

void     handle_file(void *mfile, t_pinfo *pinfo, uint32_t display)
{
  // t_pinfo             pinfo;
  // uint32_t            ncmds;
  // //  struct mach_header_64 *hd;
  // struct load_command *load_c;

  // hd = (struct mach_header_64 *)mfile;

  if (pinfo->file_type == FAT_FILE)
    handle_fat_file(&mfile, pinfo, display);
  // printf("%hhx\n", *(char *)&hd->magic);
  else if (pinfo->file_type == ARCHIVE_FILE)
    handle_archive_file(&mfile, pinfo);
  else if (pinfo->file_type == MH_FILE)
  {
    handle_macho_file(&mfile, pinfo, display);
  }
  //   ncmds = get_number_load_command(&mfile, pinfo);
//   load_c = (struct load_command *)mfile;
//   while (ncmds--)
//   {
// //    printf("load_command id %x cmd size %u\n", load_c->cmd, load_c->cmdsize);
//     handle_load_command(load_c, &pinfo, filestart);
//     load_c = (void *)load_c + load_c->cmdsize;
//   }
 //printf("end command prs\n");
  // sort_symtab(&pinfo);
  // //printf("end_sort\n");
  // assign_symbol(&pinfo, options);
  //printf("endian %s arch %d\n", pinfo.endian == 0 ? "little" : "big", pinfo.arch);
  // (void)options;
  // (void)file_name;
  //printf("magic %x cputype %d cpusubtype %d file_type %x nbr load cmd %u size of all cmd %u flags %u\n", pinfo.get_uint32_t(hd->magic), hd->cputype, hd->cpusubtype, hd->filetype, hd->ncmds, hd->sizeofcmds, hd->flags);
}
