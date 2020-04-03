#include "ft_nm.h"

uint32_t  get_number_load_command(void **mfile, t_pinfo *pinfo)
{
  uint32_t    ncmds;

  if (pinfo->arch == 64)
  {
    ncmds = pinfo->get_uint32_t((*(struct mach_header_64 **)(mfile))->ncmds);
    *mfile += sizeof(struct mach_header_64);
  }
  else
  {
    ncmds = pinfo->get_uint32_t((*(struct mach_header **)(mfile))->ncmds);
    *mfile += sizeof(struct mach_header);
  }
  return (ncmds);
}

void  handle_macho_file(void **mfile, void *filestart, t_pinfo *pinfo)
{
  struct load_command *load_c;
  uint32_t            ncmds;

  ncmds = get_number_load_command(mfile, pinfo);
  load_c = *(struct load_command **)mfile;
  while (ncmds--)
  {
  //    printf("load_command id %x cmd size %u\n", load_c->cmd, load_c->cmdsize);
    handle_load_command(load_c, pinfo, filestart);
    load_c = (void *)load_c + load_c->cmdsize;
  }
}
