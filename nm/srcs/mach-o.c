#include "ft_nm.h"

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

void  handle_macho_file(void **mfile, t_pinfo *pinfo)
{
  struct load_command *load_c;
  uint32_t            ncmds;
  // void                *filestart;

  if (!check_macho_file(*mfile, pinfo))
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
//  printf("sort avant\n");
  if (!(((t_nm_options *)pinfo->options)->flags & OPT_P))
    sort_symtab(pinfo);
  //printf("sort apresm\n");
  assign_symbol(pinfo);
}
