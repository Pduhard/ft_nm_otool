#include "ft_nm.h"

t_pinfo      get_parse_info(uint32_t magic)
{
  //printf("magic: %0x\n", magic);
  if (magic == MH_MAGIC)
    return ((t_pinfo){&same_uint32_t, NULL, NULL, 0, 0, 0, 32, MH_FILE});
  if (magic == MH_CIGAM)
    return ((t_pinfo){&reverse_uint32_t, NULL, NULL, 0, 0, 1, 32, MH_FILE});
  if (magic == MH_MAGIC_64)
    return ((t_pinfo){&same_uint32_t, NULL, NULL, 0, 0, 0, 64, MH_FILE});
  if (magic == MH_CIGAM_64)
    return ((t_pinfo){&reverse_uint32_t, NULL, NULL, 0, 0, 1, 64, MH_FILE});
  if (magic == FAT_MAGIC)
    return ((t_pinfo){&same_uint32_t, NULL, NULL, 0, 0, 0, 64, FAT_FILE});
  if (magic == FAT_CIGAM)
    return ((t_pinfo){&reverse_uint32_t, NULL, NULL, 0, 0, 1, 64, FAT_FILE});
  return ((t_pinfo){NULL, NULL, NULL, 0, 0, 0, 0, MH_FILE});
}

// uim

void     display_file_sym(void *mfile, uint32_t options, char *file_name)
{
  t_pinfo             pinfo;
  void                *filestart;
  // uint32_t            ncmds;
  // //  struct mach_header_64 *hd;
  // struct load_command *load_c;

  filestart = mfile;
  // hd = (struct mach_header_64 *)mfile;
  pinfo = get_parse_info(*(uint32_t *)(mfile));
  if (pinfo.arch != 32 && pinfo.arch != 64)
      return ;
  if (pinfo.file_type == FAT_FILE)
    handle_fat_file(&mfile, &pinfo);
  // printf("%hhx\n", *(char *)&hd->magic);
  else
    handle_macho_file(&mfile, filestart, &pinfo);
//   ncmds = get_number_load_command(&mfile, pinfo);
//   load_c = (struct load_command *)mfile;
//   while (ncmds--)
//   {
// //    printf("load_command id %x cmd size %u\n", load_c->cmd, load_c->cmdsize);
//     handle_load_command(load_c, &pinfo, filestart);
//     load_c = (void *)load_c + load_c->cmdsize;
//   }
 //printf("end command prs\n");
  sort_symtab(&pinfo);
  //printf("end_sort\n");
  assign_symbol(&pinfo, options);
  //printf("endian %s arch %d\n", pinfo.endian == 0 ? "little" : "big", pinfo.arch);
  (void)options;
  (void)file_name;
  //printf("magic %x cputype %d cpusubtype %d file_type %x nbr load cmd %u size of all cmd %u flags %u\n", pinfo.get_uint32_t(hd->magic), hd->cputype, hd->cpusubtype, hd->filetype, hd->ncmds, hd->sizeofcmds, hd->flags);
}
