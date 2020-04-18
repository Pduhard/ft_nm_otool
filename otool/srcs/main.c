#include "ft_otool.h"

void print_otool(t_pinfo *pinfo, int display)
{

    printf("OK otool\n");
    (void)pinfo;
    (void)display;
}

int main(int argc, char **argv)
{
  off_t     file_size;
  t_pinfo   pinfo;
  void      *mfile;
  uint32_t  i;

  i = 1;
  (void)argc;
  // ft_putendl(argv[argc - 1]);
  while (argv[i])
  {
    if ((mfile = mapp_file(argv[i], argv[0], &file_size)))
    {
      pinfo = get_parse_info(mfile, BIN_OTOOL);
      pinfo.file_name = argv[i];
      pinfo.fsize = file_size;
      pinfo.print = &print_otool;
      //pinfo.options = (void *)&opt;
      // printf("%d\n", pinfo.file_type);
      if (pinfo.arch == 32 || pinfo.arch == 64)
        handle_file(mfile, &pinfo, argv[1] ? DISPLAY_INFO_ON : DISPLAY_INFO_OFF);
    }
    i++;
  }
  return (0);
  // if (ft_strcmp(argv[0], "./ft_otool"))
  //   handle_lc_symtab(NULL, NULL, NULL);
  // get_symbol(0, 0, 0, NULL);
  return (0);
}
