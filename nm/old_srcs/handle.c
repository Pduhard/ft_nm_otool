#include "ft_nm.h"

extern off_t fsize;
t_cpu_info   get_my_cpu_info(void)
{
    int n;
    t_cpu_info info;

    n = 1;
    info.endian = *(char *)&n == 1 ? LITTLE : BIG;
    info.arch = sizeof(void *) == 4 ? _ARCH_32 : _ARCH_64;
    return (info);
}


// uim

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
  else
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
