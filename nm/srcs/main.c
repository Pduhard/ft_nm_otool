#include "ft_nm.h"

int       check_arch_flags(t_nm_options *opt)
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

uint32_t       update_arch_flags(char ***arch_flags_addr, char *new_flag, char *prog_name)
{
    if (!new_flag)
      return (ft_return_error(OPT_ERROR, "error: %s: missing argument(s) to -arch option\n", prog_name));
    if (update_str_tab(arch_flags_addr, new_flag, prog_name))
      return (OPT_ARCH);
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
    return (OPT_T);
}

uint32_t  check_nm_flag(char **argv, uint32_t *i, void *opt_struct)
{
    t_nm_options  *options;
    char          *flag_str;
    int           flag_i;
    uint32_t      opt_flag;

    flag_str = &(argv[*i][1]);
    options = (t_nm_options *)opt_struct;
    opt_flag = 0;
  //  printf("nm _flag %s\n", flag_str);
    if (!ft_strcmp(flag_str, "arch"))
      return (update_arch_flags(&options->arch_flags, argv[++(*i)], argv[0]));
    if (!ft_strcmp(flag_str, "t"))
      return (update_format(&options->format, argv[++(*i)], argv[0]));
    while (*flag_str)
    {
      if ((flag_i = ft_strichr(VALID_FLAGS, *flag_str)) == -1)
        return (ft_return_error(OPT_ERROR, "error: %s: invalid argument -%c\n", argv[0], *flag_str));
      opt_flag |= (1 << flag_i);
      flag_str++;
    }
    if ((opt_flag & OPT_S))
    {
      if (options->segname || options->sectname)
        return (ft_return_error(OPT_ERROR, "error: %s: more than one -s option specified\n", argv[0], *flag_str));
      if (!argv[++(*i)])
      {
        if (!(options->segname = ft_strdup(""))
          || !(options->sectname = ft_strdup("")))
          return (ft_memalloc_error(OPT_ERROR, 1, argv[0]));
      }
      else
      {
        if (!argv[*i + 1])
          return (ft_return_error(OPT_ERROR, "error: %s: missing arguments to -s\n", argv[0], *flag_str));
        if (!(options->segname = ft_strdup(argv[*i]))
          || !(options->sectname = ft_strdup(argv[*i + 1])))
          return (ft_memalloc_error(OPT_ERROR, 1, argv[0]));
        ++(*i);
      }
    }
    if (((options->flags | opt_flag) & OPT_MAJ_U) && ((options->flags | opt_flag ) & OPT_U))
      return (ft_return_error(OPT_ERROR, "error: %s: can't specifiy both -U and -u\n", argv[0]));
    if (opt_flag & OPT_MAJ_A)
      opt_flag |= OPT_O;
    return (opt_flag);
}

char  **parse_arguments(char **argv, void *opt_struct,
        uint32_t (*check_valid_flag)(char **, uint32_t *, void *), uint32_t *flags)
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
  if (files == NULL && !update_str_tab(&files, "a.out", argv[0]))
    return (NULL);
  return (files);
}

void  create_section_start(t_pinfo *pinfo)
{
    t_symtab  sect_start;
    t_symtab  *new_tab;
    // uint32_
    t_nm_options  *opt;
    uint8_t   n_sect;
    // uint64_t  n_value;
    uint32_t  i;

    opt = (t_nm_options *)pinfo->options;
    i = 0;
    n_sect = 0;
    while (i < pinfo->secid)
    {
      if (!ft_strcmp(opt->segname, ((struct section*)(pinfo->sectab + i)->secaddr)->segname)
        && !ft_strcmp(opt->sectname, ((struct section*)(pinfo->sectab + i)->secaddr)->sectname))
        n_sect = i + 1;
      i++;
    }
    // printf("%d\n", n_sect);
    if (!n_sect)
      return ;
    if (pinfo->arch == ARCH_32 && !pinfo->get_uint32_t(((struct section *)(pinfo->sectab + n_sect - 1)->secaddr)->size))
      return ;
    if (pinfo->arch == ARCH_64 && !pinfo->get_uint64_t(((struct section_64 *)(pinfo->sectab + n_sect - 1)->secaddr)->size))
        return ;
    if (!(new_tab = ft_memalloc(sizeof(t_symtab) * (pinfo->symid + 2))))
      return ;
    ft_memcpy(new_tab, pinfo->symtab, sizeof(t_symtab) * pinfo->symid);
    ft_bzero(&sect_start, sizeof(t_symtab));
    sect_start.name = ft_strdup(".section_start");
    sect_start.sym.n_type = N_SECT;
    sect_start.sym.n_sect = n_sect;
    sect_start.sym.n_value = pinfo->arch == ARCH_32 ? pinfo->get_uint32_t(((struct section *)(pinfo->sectab + n_sect - 1)->secaddr)->addr) : pinfo->get_uint64_t(((struct section_64 *)(pinfo->sectab + n_sect - 1)->secaddr)->addr);
    sect_start.symbol = get_symbol(N_SECT, n_sect, sect_start.sym.n_value, pinfo);
    //sect_start.symbol = 'd';
    new_tab[pinfo->symid++] = sect_start;
    free(pinfo->symtab);
    pinfo->symtab = new_tab;
}

void print_nm(t_pinfo *pinfo, int display)
{
  if (!(pinfo->symtab))
    ft_fdprintf(2, "warning: %s: no name list\n", pinfo->file_name);
  else if (display == DISPLAY_INFO_ON && !(((t_nm_options *)pinfo->options)->flags & OPT_O))
    printf("\n%s:\n", pinfo->file_name);

//  printf("sort avant\n");
  if ((((t_nm_options *)pinfo->options)->flags & OPT_L) && (((t_nm_options *)pinfo->options)->flags & OPT_S) && !pinfo->section_start)
  {
    create_section_start(pinfo);
    // t_symtab *new_tab;
    //
    // if ((new_tab = ft_memalloc(sizeof(t_symtab) * (pinfo->symid + 2))))
    // {
    //   ft_memcpy(new_tab, pinfo->symtab, sizeof(t_symtab) * pinfo->symid);
    //   new_tab[pinfo->symid].name = ft_strdup(".section_start");
    //   new_tab[pinfo->symid].sym.n_type = N_SECT;
    //   new_tab[pinfo->symid].sym.n_sect = 1;
    //   new_tab[pinfo->symid].sym.n_value = 0xffffff;
    //
    //   new_tab[pinfo->symid].symbol = 'c';
    //   pinfo->symid++;
    //   free(pinfo->symtab);
    //   pinfo->symtab = new_tab;
    // }
  }
  // printf("azdazdazd\n");
  if (!(((t_nm_options *)pinfo->options)->flags & OPT_P))
    sort_symtab(pinfo);
  // printf("sort apresm\n");
  assign_symbol(pinfo);
}

int main(int argc, char **argv)
{
  off_t     file_size;
  uint32_t  i;
  void      *mfile;
  char      **files;
  t_pinfo   pinfo;
  // uint32_t  options;
  t_nm_options  opt;

  ft_bzero(&opt, sizeof(t_nm_options));
  i = 0;
  if (!(files = parse_arguments(argv, &opt, &check_nm_flag, &opt.flags)))
    return (ft_usage_error(1, NM_USAGE, argv[0]));
  if ((opt.flags & OPT_ARCH) && !check_arch_flags(&opt))
    return (ft_usage_error(1, NM_USAGE, argv[0]));


  // printf("%x\n", opt.flags);
  while (files[i])
  {
    if ((mfile = mapp_file(files[i], argv[0], &file_size, BIN_NM)))
    {
      pinfo = get_parse_info(mfile, BIN_NM);
      pinfo.file_name = files[i];
      pinfo.fsize = file_size;
      pinfo.options = (void *)&opt;
      pinfo.print = &print_nm;
      // printf("%d\n", pinfo.file_type);
      if (pinfo.arch == 32 || pinfo.arch == 64)
        handle_file(mfile, &pinfo, files[1] ? DISPLAY_INFO_ON : DISPLAY_INFO_OFF);
    }
    i++;
  }
  return (0);
  (void)argc;
}
