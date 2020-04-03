#include "ft_nm.h"


void update_fat_symtab(t_pinfo *pinfo, t_pinfo *fpinfo)
{
  t_symtab  *symtab;
  uint32_t  i;
  uint32_t  j;

  if (!(symtab = ft_memalloc(sizeof(t_symtab) * (pinfo->symid + fpinfo->symid))))
    exit(EXIT_FAILURE);
  i = 0;
  while (i < pinfo->symid)
  {
    symtab[i] = pinfo->symtab[i];
    i++;
  }
  if (i)
    free(pinfo->symtab);
  j = -1;
  while (++j < fpinfo->symid)
    symtab[i++] = fpinfo->symtab[j];
//m  printf("i = %d\n", i);
  if (j)
    free(fpinfo->symtab);
  pinfo->symid = i;
  pinfo->symtab = symtab;
  /* code */
}

void  handle_fat_file(void **mfile, t_pinfo *pinfo)
{
  uint32_t  nfat;
  struct fat_arch *fhd;
  t_pinfo   fpinfo;
  void *filestart;
  void *mcurfile;

  nfat = pinfo->get_uint32_t((*(struct fat_header **)(mfile))->nfat_arch);

  fhd = (struct fat_arch *)(*mfile + sizeof(struct fat_header));
printf("%u align %x\n", nfat, fhd->align);
  while (nfat--)
  {
  //  printf("off %u size %u\n", pinfo->get_uint32_t(fhd->offset), pinfo->get_uint32_t(fhd->size));
    struct mach_header_64 *hd = (struct mach_header_64 *)(*mfile + pinfo->get_uint32_t(fhd->offset));
    fpinfo = get_parse_info(hd->magic);
  printf("magic %x cputype %x cpusubtype %x file_type %x nbr load cmd %u size of all cmd %u flags %u\n", hd->magic, hd->cputype, hd->cpusubtype, hd->filetype, hd->ncmds, hd->sizeofcmds, hd->flags);
  printf("cputype %x cputype & cpu_arch_mask : %x cpu type x_86: %x subtype x86_64 %x\n", pinfo->get_uint32_t(fhd->cputype), pinfo->get_uint32_t(fhd->cputype) & CPU_ARCH_MASK, CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL);
    if (fpinfo.arch != 32 && fpinfo.arch != 64)
        printf("arch in file chelou magic %d\n", hd->magic);
    else
    {
      if (fpinfo.file_type == MH_FILE)
      {

        filestart = *mfile + pinfo->get_uint32_t(fhd->offset);
        mcurfile = filestart;
        handle_macho_file(&mcurfile, filestart, &fpinfo);
        printf("%d\n", fpinfo.symid);
        update_fat_symtab(pinfo, &fpinfo);
    //    printf("hallo\n");
      }
      else
        printf("magic %d in FAT file\n", hd->magic);
    }
    fhd++;
  }
  //*mfile += sizeof(struct fat_header) +  * sizeof(struct fat_arch);
}
