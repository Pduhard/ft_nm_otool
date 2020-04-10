#include "ft_nm.h"

void *mapp_file(char *file_name, char *bin_name, off_t *fsize)
{
  int         fd;
  struct stat stat_buf;
  void        *mapped_file;

  if ((fd = open(file_name, O_RDONLY)) < 0 || fstat(fd, &stat_buf) < 0)
  {
    ft_fdprintf(2, "error: %s: can't open file: %s\n", bin_name, file_name);
    return (NULL);
  }
  *fsize = stat_buf.st_size;
  if ((mapped_file = mmap(NULL, stat_buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
  {
    ft_fdprintf(2, "error: %s: can't map file: %s\n", bin_name, file_name);
    return (NULL);
  }
  close(fd);
  return (mapped_file);
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

int main(int argc, char **argv)
{
  off_t     fsize;
  uint32_t  i;
  void      *mfile;
  char      **files;
  // uint32_t  options;
  t_nm_options  opt;

  ft_bzero(&opt, sizeof(t_nm_options));
  i = 0;
  if (!(files = parse_arguments(argv, &opt, &check_nm_flag, &opt.flags)))
    return (ft_usage_error(1, NM_USAGE, argv[0])); // !!!!!! need to run with a.out !!!!!!
  // printf("%x\n", opt.flags);
  while (files[i])
  {
    if ((mfile = mapp_file(files[i], argv[0], &fsize)))
      display_file_sym(mfile, files[i], fsize, &opt);
    i++;
  }
  return (0);
  (void)argc;
}
