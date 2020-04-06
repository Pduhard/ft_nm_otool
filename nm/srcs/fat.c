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

void reverse_bytes(void *dst, void *src, size_t size)
{
  unsigned char *a;
  unsigned char *b;
  uint32_t      i;
  uint32_t      j;

  i = 0;
  j = size - 1;
  a = (unsigned char *)dst;
  b = (unsigned char *)src;
  while (i < size)
    a[i++] = b[j--];
}

struct fat_arch *get_best_fat_arch(t_pinfo *pinfo, struct fat_header *fat_hd, struct fat_arch *fat_archs, struct fat_arch **fat_archs_rev)
{
  uint32_t        i;
  struct fat_arch *best;
  uint32_t      nfat_arch;

  if (fat_hd->magic == FAT_MAGIC)
  {
    return (NXFindBestFatArch(pinfo->myinfo->cputype, pinfo->myinfo->cpusubtype, fat_archs, fat_hd->nfat_arch));
  }
  else
  {
    nfat_arch = pinfo->get_uint32_t(fat_hd->nfat_arch);
    if (!((*fat_archs_rev) = ft_memalloc(sizeof(struct fat_arch) * nfat_arch)))
      return (NULL);
    i = 0;
    while (i < nfat_arch)
    {
    //  printf("%08x %08x %08x %08x %08x to \n", fat_archs[i].cputype, fat_archs[i].cpusubtype, fat_archs[i].offset, fat_archs[i].size, fat_archs[i].align);
      reverse_bytes(&((*fat_archs_rev)[i].cputype), &(fat_archs[i].cputype), sizeof(cpu_type_t));
      reverse_bytes(&((*fat_archs_rev)[i].cpusubtype), &(fat_archs[i].cpusubtype), sizeof(cpu_subtype_t));
      reverse_bytes(&((*fat_archs_rev)[i].offset), &(fat_archs[i].offset), sizeof(uint32_t));
      reverse_bytes(&((*fat_archs_rev)[i].size), &(fat_archs[i].size), sizeof(uint32_t));
      reverse_bytes(&((*fat_archs_rev)[i].align), &(fat_archs[i].align), sizeof(uint32_t));
    //  printf("%08x %08x %08x %08x %08x\n\n", fat_archs_rev[i].cputype, fat_archs_rev[i].cpusubtype, fat_archs_rev[i].offset, fat_archs_rev[i].size, fat_archs_rev[i].align);
      i++;
    }
  //  printf("nmae %s %08x %08x\n",pinfo->myinfo->name, pinfo->myinfo->cputype, pinfo->myinfo->cpusubtype);
    best = NXFindBestFatArch(pinfo->myinfo->cputype, pinfo->myinfo->cpusubtype, *fat_archs_rev, nfat_arch);
    //free(fat_archs_rev);
  //m printf("salut %p\n", best);
    return (best);
  }
}

void handle_fat_arch(struct mach_header_64 *hd, uint32_t options, t_pinfo *pinfo, uint32_t display)
{
  t_pinfo                 fpinfo;
  void                    *mcurfile;
  const NXArchInfo        *farch_info;

//  printf("%um\n", fat_arch->offset);
  fpinfo = get_parse_info((void *)hd);
//  printf("HALLO %x\n", hd->magic);
  if (display == FAT_ARCH_ALL)
  {
//    printf("HALLO\n");
     if ((farch_info = NXGetArchInfoFromCpuType(fpinfo.get_uint32_t(hd->cputype), fpinfo.get_uint32_t(hd->cpusubtype))))
      printf("\n%s (for architecture %s):\n", pinfo->file_name, farch_info->name);
    else
     printf("\n%s (for architecture UNKNOWN)\nm", pinfo->file_name);
//    printf("HLLO\n");
  }
  if (fpinfo.arch != 32 && fpinfo.arch != 64)
      printf("arch in file chelou magic %d\n", hd->magic);
  else
  {
    if (fpinfo.file_type == MH_FILE)
    {
      mcurfile = (void *)hd;
      // printf("HFLUzefzefzefzef\n");
      // printf("%d %d\n", fpinfo.endian, fpinfo.arch);
      handle_macho_file(&mcurfile, (void *)hd, &fpinfo, options);
      // update_fat_symtab(pinfo, &fpinfo);
    }
    else
      printf("magic %d in FAT file\n", hd->magic);
  }
}

int check_fat_file(void *mfile, t_pinfo *pinfo)
{
  struct fat_header *fat_hd;
  struct fat_arch   *fat_arch;
  uint32_t          nfat_file;
  uint32_t          i;

  i = 0;
  fat_hd = (struct fat_header *)mfile;
  fat_arch = (struct fat_arch *)(mfile + sizeof(struct fat_header));
  nfat_file = pinfo->get_uint32_t(fat_hd->nfat_arch);
  if (!nfat_file)
  {
    ft_fdprintf(2, "fat file: %s malformed (contains zero architecture types)\n", pinfo->file_name);
    return (0);
  }
  while (i < nfat_file)
  {
    if (pinfo->get_uint32_t((fat_arch + i)->offset) + pinfo->get_uint32_t((fat_arch + i)->size) > pinfo->fsize)
    {
      ft_fdprintf(2, "fat file: %s truncated or malformed (offset plus size of cputype (%d) cpusubtype (%d) extends past the end of the file)\n", pinfo->file_name, pinfo->get_uint32_t((fat_arch + i)->cputype), pinfo->get_uint32_t((fat_arch + i)->cpusubtype));
      return (0);
    }
    i++;
  }
  return (1);
}

void  handle_fat_file(void **mfile, t_pinfo *pinfo, uint32_t options)
{
  struct fat_header *fat_hd;
  struct fat_arch   *fat_arch;
  struct fat_arch   *fat_archs_rev;
  uint32_t          i;
  //uint32_t  nfat;
//  struct fat_arch   *fhd;
  // t_pinfo   fpinfo;
  // void *filestart;
  // void *mcurfile;
//  const NXArchInfo *i = NXGetAllArchInfos();
//  const NXArchInfo *my = NXGetLocalArchInfo();
//  struct fat_arch *b;

  //exit(0);
  if (!check_fat_file(*mfile, pinfo))
    return ;
  i = 0;
  fat_archs_rev = NULL;
  fat_hd = *(struct fat_header **)(mfile);
//m  printf("%x\n", fat_hd->magic);
  if (!(options & OPT_ARCH))
  {
    fat_arch = get_best_fat_arch(pinfo, fat_hd, (struct fat_arch *)(*mfile + sizeof(struct fat_header)), &fat_archs_rev);
    if (fat_arch)
      handle_fat_arch((struct mach_header_64 *)(*mfile + fat_arch->offset), options, pinfo, FAT_ARCH_SPEC);
    else
    {
      fat_arch = (struct fat_arch *)(*mfile + sizeof(struct fat_header));
      while (i < pinfo->get_uint32_t(fat_hd->nfat_arch))
      {
          handle_fat_arch((struct mach_header_64 *)(*mfile + pinfo->get_uint32_t((fat_arch + i)->offset)), options, pinfo, FAT_ARCH_ALL);
          i++;
      }
    }
    //printf("best cputype %x cpusub %x\n", fat_arch->cputype, fat_arch->cpusubtype);
    //printf("info cputype %x cpusub %x\n", pinfo->myinfo->cputype, pinfo->myinfo->cpusubtype);
  }

  if (fat_archs_rev)
    free(fat_archs_rev);
/*  while (i->name)
  {
    printf("%p arch name %s desc %s\n", i, i->name, i->description);
    i++;
  }
  printf("my %p arch name %s desc %s\n", my, my->name, my->description);
  nfat = pinfo->get_uint32_t((*(struct fat_header **)(mfile))->nfat_arch);
  printf("%zu == 4 ?\n", sizeof(void *));
  fhd = (struct fat_arch *)(*mfile + sizeof(struct fat_header));
  printf("%u align %x magic %x\n", nfat, fhd->align, (*(struct fat_header **)(mfile))->magic);

  // b = NXFindBestFatArch(my->cputype,
  //         my->cpusubtype,
  //         fhd,
  //         nfat);
  //
  // printf("%x %x \n",b->cputype, b->cpusubtype);
  while (nfat--)
  {
  //  printf("off %u size %u\n", pinfo->get_uint32_t(fhd->offset), pinfo->get_uint32_t(fhd->size));
    struct mach_header_64 *hd = (struct mach_header_64 *)(*mfile + pinfo->get_uint32_t(fhd->offset));
    fpinfo = get_parse_info(hd->magic);
  printf("magic %x cputype %x cpusubtype %x file_type %x nbr load cmd %u size of all cmd %u flags %u\n", hd->magic, hd->cputype, hd->cpusubtype, hd->filetype, hd->ncmds, hd->sizeofcmds, hd->flags);
  printf("cputype %x cpusub type %x\n", pinfo->get_uint32_t(fhd->cputype), pinfo->get_uint32_t(fhd->cpusubtype));
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
  */
  //*mfile += sizeof(struct fat_header) +  * sizeof(struct fat_arch);
}
