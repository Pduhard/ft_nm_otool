#include "common.h"

void *mapp_file(char *file_name, char *bin_name, off_t *file_size)
{
  int         fd;
  struct stat stat_buf;
  void        *mapped_file;

  if ((fd = open(file_name, O_RDONLY)) < 0 || fstat(fd, &stat_buf) < 0)
  {
    ft_fdprintf(2, "error: %s: can't open file: %s\n", bin_name, file_name);
    return (NULL);
  }
  *file_size = stat_buf.st_size;
  if ((mapped_file = mmap(NULL, stat_buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
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
  char      *magic_str;

  magic = *(uint32_t *)mfile;
  magic_str = (char *)mfile;
//  printf("magic: %0x\n", magic);
  if (magic == MH_MAGIC)
    return ((t_pinfo){&same_uint16_t, &same_uint32_t, &same_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 0, 32, bin, NULL, MH_FILE, NULL, 0, NULL, NULL, 0, mfile});
  if (magic == MH_CIGAM)
    return ((t_pinfo){&reverse_uint16_t, &reverse_uint32_t, &reverse_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 1, 32, bin, NULL, MH_FILE, NULL, 0, NULL, NULL, 0, mfile});
  if (magic == MH_MAGIC_64)
    return ((t_pinfo){&same_uint16_t, &same_uint32_t, &same_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 0, 64, bin, NULL, MH_FILE, NULL, 0, NULL, NULL, 0, mfile});
  if (magic == MH_CIGAM_64)
    return ((t_pinfo){&reverse_uint16_t, &reverse_uint32_t, &reverse_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 1, 64, bin, NULL, MH_FILE, NULL, 0, NULL, NULL, 0, mfile});
  if (magic == FAT_MAGIC)
    return ((t_pinfo){&same_uint16_t, &same_uint32_t, &same_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 0, 64, bin, NULL, FAT_FILE, NULL, 0, NULL, NULL, 0, mfile});
  if (magic == FAT_CIGAM)
    return ((t_pinfo){&reverse_uint16_t, &reverse_uint32_t, &reverse_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 1, 64, bin, NULL, FAT_FILE, NULL, 0, NULL, NULL, 0, mfile});
  if (!ft_strncmp(magic_str, ARMAG, SARMAG))
    return ((t_pinfo){&same_uint16_t, &same_uint32_t, &same_uint64_t, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 1, 64, bin, NULL, ARCHIVE_FILE, NULL, 0, NULL, NULL, 0, mfile});
  return ((t_pinfo){NULL, NULL, NULL, NULL, get_local_arch_info(), NULL, NULL, 0, 0, 0, 0, bin, NULL, UNKNOWN_FILE, NULL, 0, NULL, NULL, 0, mfile});
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
