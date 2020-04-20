#include "common.h"

// extern off_t fsize;
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


struct fat_arch *get_corr_fat_arch(const NXArchInfo *fat_arch_info, struct fat_header *fat_hd, struct fat_arch *fat_archs)
{
  uint32_t  i;
  uint32_t  (*get)(uint32_t);
  uint32_t      nfat_arch;

  i = 0;
  if (fat_hd->magic == FAT_MAGIC)
    get = &same_uint32_t;
  else
    get = &reverse_uint32_t;
  nfat_arch = get(fat_hd->nfat_arch);

  while (i < nfat_arch)
  {
  //  arch_info = NXGetArchInfoFromName(arch_name);
    // if (!((t_nm_options *)pinfo->options)->arch_flags[1])
    // {
    //    if (!(all_info = NXGetAllArchInfos()))
    //      return (0);
    //    family = NULL;
    //    while (all_info->name)
    //    {
    //      if (all_info->cputype == arch_info->cputype)
    //      {
    //        // printf("%x %x ?\n", (all_info->cpusubtype & ~CPU_SUBTYPE_MASK), (arch_info->cpusubtype & ~CPU_SUBTYPE_MASK));
    //        if ((all_info->cpusubtype & ~CPU_SUBTYPE_MASK) == (arch_info->cpusubtype & ~CPU_SUBTYPE_MASK))
    //          family = all_info;
    //        break ;
    //      }
    //      ++all_info;
    //    }
    // }

    if ((int)(get((fat_archs + i)->cputype)) == fat_arch_info->cputype && (int)(get((fat_archs + i)->cpusubtype) & ~CPU_SUBTYPE_MASK) == (fat_arch_info->cpusubtype & ~CPU_SUBTYPE_MASK))
      return (fat_archs + i);
    // if ((int)(get((fat_archs + i)->cputype)) == fat_arch_info->cputype && (int)get((fat_archs + i)->cpusubtype) == fat_arch_info->cpusubtype)
    //   return (fat_archs + i);
    i++;
  }
  // printf("HAAAA\n");m
  return (NULL);
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
//      printf("%08x %08x %08x %08x %08x to \n", fat_archs[i].cputype, fat_archs[i].cpusubtype, fat_archs[i].offset, fat_archs[i].size, fat_archs[i].align);
      reverse_bytes(&((*fat_archs_rev)[i].cputype), &(fat_archs[i].cputype), sizeof(cpu_type_t));
      reverse_bytes(&((*fat_archs_rev)[i].cpusubtype), &(fat_archs[i].cpusubtype), sizeof(cpu_subtype_t));
      reverse_bytes(&((*fat_archs_rev)[i].offset), &(fat_archs[i].offset), sizeof(uint32_t));
      reverse_bytes(&((*fat_archs_rev)[i].size), &(fat_archs[i].size), sizeof(uint32_t));
      reverse_bytes(&((*fat_archs_rev)[i].align), &(fat_archs[i].align), sizeof(uint32_t));
//      printf("%08x %08x %08x %08d %08x\n\n", (*fat_archs_rev)[i].cputype, (*fat_archs_rev)[i].cpusubtype, (*fat_archs_rev)[i].offset, (*fat_archs_rev)[i].size, (*fat_archs_rev)[i].align);
      i++;
    }
  //  printf("nmae %s %08x %08x\n",pinfo->myinfo->name, pinfo->myinfo->cputype, pinfo->myinfo->cpusubtype);
    best = NXFindBestFatArch(pinfo->myinfo->cputype, pinfo->myinfo->cpusubtype, *fat_archs_rev, nfat_arch);
    //free(fat_archs_rev);
  //m printf("salut %p\n", best);
//  printf("%08x %08x %08x %08d %08x\n\n", best->cputype, best->cpusubtype, best->offset, best->size, best->align);

    return (best);
  }
}

void handle_fat_arch(struct mach_header_64 *hd, t_pinfo fpinfo, t_pinfo *pinfo, uint32_t display)
{
  // t_pinfo                 fpinfo;
  void                    *mcurfile;
  // const NXArchInfo        *farch_info;

    // printf("%x\n", display);
  // fpinfo = get_parse_info((void *)hd);
  //mprintf("HALLO %s\n", (char *)&mhd->magic);
  if (!fpinfo.get_uint32_t)
    return ;
  // if (pinfo->fat_arch_from && (display & FAT_ARCH_ALL))
  // {
  //   printf("DUDE\n");
  //   fpinfo.fat_arch_from = pinfo->fat_arch_from;
  // }
  // if (display == FAT_ARCH_ALL)
  // {
    // printf("%x %x\n", hd->cputype, hd->cpusubtype);
  // if ((farch_info = NXGetArchInfoFromCpuType(fpinfo.get_uint32_t(hd->cputype), fpinfo.get_uint32_t(hd->cpusubtype))))
  // {
  //   if ((display & FAT_ARCH_ALL))
  //     fpinfo.fat_arch_from = (char *)farch_info->name;
    // if ((display & DISPLAY_INFO_ON))
    //   printf()
    // if (!(((t_nm_options *)pinfo->options)->flags & OPT_O))
    //   printf("\n%s (for architecture %s):\n", pinfo->file_name, farch_info->name);
  if (fpinfo.file_type != ARCHIVE_FILE && fpinfo.bin == BIN_NM)
  {
    if (!(((t_nm_options *)pinfo->options)->flags & OPT_O) && (display & (DISPLAY_INFO_ON | FAT_ARCH_ALL)))
      printf("\n%s", pinfo->file_name);
    if (!(((t_nm_options *)pinfo->options)->flags & OPT_O) && (display & FAT_ARCH_ALL))
      printf(" (for architecture %s):\n", fpinfo.fat_arch_from);
    else if (!(((t_nm_options *)pinfo->options)->flags & OPT_O) && (display & DISPLAY_INFO_ON))
      printf(":\n");
  }
  // }

    //else
    // printf("\n%s (for architecture UNKNOWN)\n", pinfo->file_name);
    // printf("HLLO\n");
  // }
  fpinfo.file_name = pinfo->file_name;
  fpinfo.options = pinfo->options;
  fpinfo.print = pinfo->print;
  // fsize = size;

  if (fpinfo.arch != 32 && fpinfo.arch != 64)
      ;//printf("arch in file chelou magic %d\n", hd->magic);

  else
  {
    if (fpinfo.file_type == MH_FILE)
    {

      mcurfile = (void *)hd;
       // printf("HFLUzefzefzefzef\n");
      // printf("%d %d\n", fpinfo.endian, fpinfo.arch);
      handle_macho_file(&mcurfile, &fpinfo, 0);
      // update_fat_symtab(pinfo, &fpinfo);
    }
    else if (fpinfo.file_type == ARCHIVE_FILE)
    {
      mcurfile = (void *)hd;
      handle_archive_file(&mcurfile, &fpinfo);
    }
      //printf("magic %d in FAT file\n", hd->magic);
  }
}

int check_fat_file(void *mfile, t_pinfo *pinfo)
{
  struct mach_header_64  *hd;
  struct fat_header *fat_hd;
  struct fat_arch   *fat_arch;
  uint32_t          nfat_file;
  uint32_t          i;
  uint32_t          j;
  t_pinfo           fpinfo;

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
    if (pinfo->get_uint32_t((fat_arch + i)->align) > sizeof(uint32_t) * 8)
    {
      ft_fdprintf(2, "fat file: %s alignment (2^%u) too large for cputype (%d) cpusubtype (%d) (maximum 2^%zu)\n)", pinfo->file_name, pinfo->get_uint32_t((fat_arch + i)->align), pinfo->get_uint32_t((fat_arch + i)->cputype), pinfo->get_uint32_t((fat_arch + i)->cpusubtype), sizeof(uint32_t) * 8);

      return (0);
    }
    if ((pinfo->get_uint32_t((fat_arch + i)->offset) % (1 << pinfo->get_uint32_t((fat_arch + i)->align))))
    {
      ft_fdprintf(2, "fat file: %s offset: %u for cputype (%d) cpusubtype (%d) not aligned on it's alignment (2^%u)\n", pinfo->file_name, pinfo->get_uint32_t((fat_arch + i)->offset), pinfo->get_uint32_t((fat_arch + i)->cputype), pinfo->get_uint32_t((fat_arch + i)->cpusubtype), pinfo->get_uint32_t((fat_arch + i)->align));
      return (0);
    }
    j = 0;
    while (j < nfat_file)
    {
      if (i != j && pinfo->get_uint32_t((fat_arch + i)->cputype) == pinfo->get_uint32_t((fat_arch + j)->cputype)
        && pinfo->get_uint32_t((fat_arch + i)->cpusubtype) == pinfo->get_uint32_t((fat_arch + j)->cpusubtype))
      {
        ft_fdprintf(2, "fat file: %s contains two of the same architecture (cputype (%d) cpusubtype (%d))\n",  pinfo->file_name, pinfo->get_uint32_t((fat_arch + i)->cputype), pinfo->get_uint32_t((fat_arch + i)->cpusubtype));
        return (0);
      }
      j++;
    }
    hd = (struct mach_header_64 *)(mfile + pinfo->get_uint32_t((fat_arch + i)->offset));
    fpinfo = get_parse_info((void *)hd, pinfo->bin);
    fpinfo.print = pinfo->print;
    if (fpinfo.file_type == MH_FILE && pinfo->get_uint32_t((fat_arch + i)->cputype) != fpinfo.get_uint32_t(hd->cputype))
    {
      ft_fdprintf(2, "%s malformed fat file (fat header architecture: %u's cputype does not match object file's mach header)\n", pinfo->file_name, i);
      //return (0);
    }
    fpinfo.fsize = pinfo->get_uint32_t((fat_arch + i)->size);
    fpinfo.file_name = pinfo->file_name;
    fpinfo.options = pinfo->options;
    fpinfo.fat_arch_from = pinfo->file_name;
    // printf("bjr\n");
    if (fpinfo.file_type == MH_FILE && !check_macho_file(hd, &fpinfo))
    {
      // printf("HAAAAA\n");
      return (0);

    }
    else if (fpinfo.file_type == ARCHIVE_FILE && !check_archive_file(hd, &fpinfo))
      return (0);
    i++;
  }
  return (1);
}

void  handle_fat_file(void **mfile, t_pinfo *pinfo, uint32_t display)
{
  struct fat_header *fat_hd;
  struct fat_arch   *fat_arch;
  struct fat_arch   *fat_archs_rev;
  char              **arch_flags;
  const NXArchInfo  *fat_arch_info;
  uint32_t          i;
  uint32_t          flags;
  t_pinfo           fpinfo;

  flags = pinfo->bin == BIN_NM ? ((t_nm_options *)pinfo->options)->flags : 0;
  //uint32_t  nfat;
//  struct fat_arch   *fhd;
  // t_pinfo   fpinfo;
  // void *filestart;
  // void *mcurfile;
//  const NXArchInfo *i = NXGetAllArchInfos();
//  const NXArchInfo *my = NXGetLocalArchInfo();
//  struct fat_arch *b;

  //exit(0);

  // printf("OUPS\n");
  if (pinfo->bin == BIN_NM && !check_fat_file(*mfile, pinfo))
  {
      return ;
  }
  i = 0;
  fat_archs_rev = NULL;
  fat_hd = *(struct fat_header **)(mfile);
//m  printf("%x\n", fat_hd->magic);
  if (!(flags & OPT_ARCH) && (fat_arch = get_best_fat_arch(pinfo, fat_hd, (struct fat_arch *)(*mfile + sizeof(struct fat_header)), &fat_archs_rev)))
  {
    // printf("zzzzzzzzzz\n");m
    // fat_arch_info = NXGetArchInfoFromCpuType(fat_arch->cputype), fat_arch->cpusubtype));
    fpinfo = get_parse_info((struct mach_header_64 *)(*mfile + fat_arch->offset), pinfo->bin);
    fpinfo.print = pinfo->print;
    // pinfo->fat_arch_from = NULL;
    //
    // fat_arch_info = NXGetArchInfoFromCpuType(fat_arch->cputype, fat_arch->cpusubtype);
    //
    // if (!ft_strncmp(fat_arch_info->description, "Intel family", 12) || !ft_strncmp(fat_arch_info->description, "PowerPC cpusubtype", 18))
    //   ft_getf(&pinfo->fat_arch_from, "cputype (%u) cpusubtype (%u)\n", fat_arch_info->cputype, fat_arch_info->cpusubtype);
    // else
    //   pinfo->fat_arch_from = (char *)fat_arch_info->name;
    // fpinfo.fat_arch_from = pinfo->fat_arch_from;

    fpinfo.fsize = fat_arch->size;
    if ((fpinfo.file_type == ARCHIVE_FILE || ( fpinfo.file_type == MH_FILE && fpinfo.get_uint32_t( ((struct mach_header_64 *)(*mfile + fat_arch->offset))->cputype ) == (uint32_t)fat_arch->cputype)))
      handle_fat_arch((struct mach_header_64 *)(*mfile + fat_arch->offset), fpinfo, pinfo, (display | FAT_ARCH_SPEC));
    else
      ft_fdprintf(2, "%s malformed fat file (fat header architecture: %u's cputype does not match object file's mach header)\n", pinfo->file_name, i);

  }
  else if (!(flags & OPT_ARCH) || !ft_strcmp(((t_nm_options *)pinfo->options)->arch_flags[0], "all"))
  {
    fat_arch = (struct fat_arch *)(*mfile + sizeof(struct fat_header));
  //  fat_arch_info = NXGetArchInfoFromCpuType(pinfo->get_uint32_t(fat_arch->cputype), pinfo->get_uint32_t(fat_arch->cpusubtype));
    // printf("%p\n", fat_arch_info)
  //  pinfo->fat_arch_from = (char *)fat_arch_info->name;
    while (i < pinfo->get_uint32_t(fat_hd->nfat_arch))
    {
      // printf("mqdqzd\n");
      if ((fat_arch_info = NXGetArchInfoFromCpuType(pinfo->get_uint32_t((fat_arch + i)->cputype), pinfo->get_uint32_t((fat_arch + i)->cpusubtype))))
      {
        fpinfo = get_parse_info((struct mach_header_64 *)(*mfile + pinfo->get_uint32_t((fat_arch + i)->offset)), pinfo->bin);
        fpinfo.print = pinfo->print;
       // printf("%d %d %s \n", pinfo->get_uint32_t((fat_arch + i)->cpusubtype), fat_arch_info->cpusubtype, fat_arch_info->description);
        pinfo->fat_arch_from = NULL;
        if (!ft_strncmp(fat_arch_info->description, "Intel family", 12) || !ft_strncmp(fat_arch_info->description, "PowerPC cpusubtype", 18))
          ft_getf(&pinfo->fat_arch_from, "cputype (%u) cpusubtype (%u)", fat_arch_info->cputype, fat_arch_info->cpusubtype);
        else
          pinfo->fat_arch_from = (char *)fat_arch_info->name;
        fpinfo.fat_arch_from = pinfo->fat_arch_from;
        // printf("%s\n", fpinfo.fat_arch_from);
        fpinfo.fsize = pinfo->get_uint32_t((fat_arch + i)->size);
        if ((fpinfo.file_type == ARCHIVE_FILE || ( fpinfo.file_type == MH_FILE && fpinfo.get_uint32_t( ((struct mach_header_64 *)(*mfile + pinfo->get_uint32_t((fat_arch + i)->offset)))->cputype ) == pinfo->get_uint32_t((fat_arch + i)->cputype))))
          handle_fat_arch((struct mach_header_64 *)(*mfile + pinfo->get_uint32_t((fat_arch + i)->offset)), fpinfo, pinfo, (display | FAT_ARCH_ALL));
        else
          ft_fdprintf(2, "%s malformed fat file (fat header architecture: %u's cputype does not match object file's mach header)\n", pinfo->file_name, i);

      }
      i++;
    }
  }
  else
  {
    arch_flags = ((t_nm_options *)pinfo->options)->arch_flags;
    i = 0;
    // printf("éNnnnnn\n");
    while (arch_flags[i])
    {
      if (fat_archs_rev)
        free(fat_archs_rev);
      fat_arch_info = NXGetArchInfoFromName(arch_flags[i]);
      if ((fat_arch = get_corr_fat_arch(fat_arch_info, fat_hd, (struct fat_arch *)(*mfile + sizeof(struct fat_header)))))
      {
        pinfo->fat_arch_from = NULL;
        if (!ft_strncmp(fat_arch_info->description, "Intel family", sizeof("Intel family")) || !ft_strncmp(fat_arch_info->description, "PowerPC cpusubtype", sizeof("PowerPC cpusubtype")))
          ft_getf(&pinfo->fat_arch_from, "cputype (%u) cpusubtype (%u)", fat_arch_info->cputype, fat_arch_info->cpusubtype);
        else
          pinfo->fat_arch_from = (char *)fat_arch_info->name;
        // printf("%s %x\n", arch_flags[1], display);

        fpinfo = get_parse_info((struct mach_header_64 *)(*mfile + pinfo->get_uint32_t(fat_arch->offset)), pinfo->bin);
        fpinfo.print = pinfo->print;
        fpinfo.fat_arch_from = arch_flags[1] ? pinfo->fat_arch_from : NULL;
        fpinfo.fsize = pinfo->get_uint32_t(fat_arch->size);
        if ((fpinfo.file_type == ARCHIVE_FILE || ( fpinfo.file_type == MH_FILE && fpinfo.get_uint32_t( ((struct mach_header_64 *)(*mfile + pinfo->get_uint32_t(fat_arch->offset)))->cputype ) == pinfo->get_uint32_t(fat_arch->cputype))))
          handle_fat_arch((struct mach_header_64 *)(*mfile + pinfo->get_uint32_t(fat_arch->offset)), fpinfo, pinfo, arch_flags[1] ? (display | FAT_ARCH_ALL) : (display | FAT_ARCH_SPEC));
        else
          ft_fdprintf(2, "%s malformed fat file (fat header architecture: %u's cputype does not match object file's mach header)\n", pinfo->file_name, i);


      }
      else
      {
        ft_fdprintf(2, "does not contain architecture: %s\n", fat_arch_info->name);
      }
      i++;
    }
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
