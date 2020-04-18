#include "ft_nm.h"

extern off_t fsize;

int   check_archive_file(void *mfile, t_pinfo *pinfo)
{
  struct ar_hdr *ar_hd;
  cpu_type_t    cputype;
  cpu_subtype_t    cpusubtype;
  void          *hd;
  t_pinfo       fpinfo;
  uint32_t      check;
  uint32_t      final_check;
  uint32_t      i;
  t_nm_options  *options;

  options = (t_nm_options *)pinfo->options;

  final_check = 0;
  i = 0;
  while ((!i && !(options->flags & OPT_ARCH)) || ((options->flags & OPT_ARCH) && options->arch_flags[i]))
  {
    check = (options->flags & OPT_ARCH) && !pinfo->fat_arch_from ? 0 : 1;
    if (!check && !ft_strcmp(options->arch_flags[0], "all"))
      check = 1;
    cputype = 0;
     // printf("M\n");
    // if (!check_arch_in_file(mfile, pinfo))
      // return (0);
    if ((off_t)SARMAG == pinfo->fsize)
      return (1);
    if ((off_t)(SARMAG + sizeof(struct ar_hdr)) > pinfo->fsize)
    {
    //  printf("ér\n");m
      ft_fdprintf(2, "truncated or malformed (archive header of first member extends past the end of the file)\n");
      return (0);
    }

    ar_hd = (struct ar_hdr *)(mfile + SARMAG);
    if ((off_t)((void *)ar_hd - mfile + ft_atoll(ar_hd->ar_size)) > pinfo->fsize)
    {
      ft_fdprintf(2, "size too large (archive member extends past the end of the file)\n");
      return (0);
    }
    uint32_t extended = ft_strncmp(ar_hd->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) ? 0 : 1;
    while ((off_t)((void *)ar_hd - mfile) < pinfo->fsize)
    {
        // if (!(extended && (!ft_strcmp(name, SYMDEF) || !ft_strcmp(name, SYMDEF_SORTED))))
        // {
        // }

      //  printf("%u %u sizeof %zu\n", (unsigned int)((void *)ar_hd - mfile + ft_atoll(ar_hd->ar_size)), (unsigned int)pinfo->fsize, sizeof(struct ar_hdr));
        hd = (void *)ar_hd + sizeof(struct ar_hdr) + (extended ? ft_atoll(ar_hd->ar_name + ft_strlen(AR_EFMT1)) : 0);
        if ((off_t)((void *)hd - mfile) > pinfo->fsize)
        {
          ft_fdprintf(2, "size too large (archive member extends past the end of the file)\n");
          return (0);
        }
        fpinfo = get_parse_info((void *)hd);
        fpinfo.file_name = pinfo->file_name;
        fpinfo.options = pinfo->options;
    //   printf("magic %x == %x ?\n", *(uint32_t *)ar_hd, FAT_MAGIC);
        if (*(uint32_t *)hd == FAT_MAGIC || *(uint32_t *)hd == FAT_CIGAM)
        {
          ft_fdprintf(2, "is a fat file (not allowed in an archive)\n");
          return (0);
        }
        else if (*(uint32_t *)hd == MH_MAGIC || *(uint32_t *)hd == MH_CIGAM)
        {
          if (cputype == 0)
          {
            cputype = fpinfo.get_uint32_t(((struct mach_header *)hd)->cputype);
            cpusubtype = fpinfo.get_uint32_t(((struct mach_header *)hd)->cpusubtype);
          }
          if (!check)
            check |= check_arch_in_file(cputype, cpusubtype, &fpinfo, options->arch_flags[i]);
          else if ((uint32_t)cputype != fpinfo.get_uint32_t(((struct mach_header *)hd)->cputype))
            ft_fdprintf(2, "cputype (%d) does not match previous archive members cputype (%d) (all members must match)\n", fpinfo.get_uint32_t(((struct mach_header *)hd)->cputype), cputype);
        }
        else if (*(uint32_t *)hd == MH_MAGIC_64 || *(uint32_t *)hd == MH_CIGAM_64)
        {
          if (cputype == 0)
          {
            cputype = fpinfo.get_uint32_t(((struct mach_header_64 *)hd)->cputype);
            cpusubtype = fpinfo.get_uint32_t(((struct mach_header_64 *)hd)->cpusubtype);
          }
          if (!check)
            check |= check_arch_in_file(cputype, cpusubtype, &fpinfo, options->arch_flags[i]);
          else if ((uint32_t)cputype != fpinfo.get_uint32_t(((struct mach_header_64 *)hd)->cputype))
            ft_fdprintf(2, "cputype (%d) does not match previous archive members cputype (%d) (all members must match)\n", fpinfo.get_uint32_t(((struct mach_header_64 *)hd)->cputype), cputype);
        }
        ar_hd = (struct ar_hdr *)((void *)ar_hd + ft_atoll(ar_hd->ar_size) + sizeof(struct ar_hdr));
        if ((off_t)((void *)ar_hd - mfile + ft_atoll(ar_hd->ar_size)) > pinfo->fsize)
        {
          ft_fdprintf(2, "size too large (archive member extends past the end of the file)\n");
          return (0);
        }
    }
    final_check |= check;
    if (!check)
      ft_fdprintf(2, "file: %s does not contain architecture %s\n", pinfo->file_name, options->arch_flags[i]);
    i++;
  }
  // if (!check)
  //   ft_fdprintf(2, "does not contain architecture: i386")
  // printf("????????\n");
  return (final_check);
}

char  *get_ar_name(struct ar_hdr *ar_hd, uint32_t *extended)
{
  uint32_t  i;

  if ((*extended = ft_strncmp(ar_hd->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) ? 0 : 1))
    return (ft_strdup((char *)(ar_hd + 1)));
  i = 0;
  while (ar_hd->ar_name[i] != ' ' && i < sizeof(ar_hd->ar_name))
    ++i;
  return (ft_strndup(ar_hd->ar_name, i));
}

void  handle_archive_file(void **mfile, t_pinfo *pinfo)
{
  struct ar_hdr *ar_hd;
  void          *mcurfile;
  struct mach_header_64 *hd;
  t_pinfo       fpinfo;
  uint32_t      extended;
  char          *name;


 if (!check_archive_file(*mfile, pinfo))
   return ;
  ar_hd = (struct ar_hdr *)(*mfile + SARMAG);
//  printf("%zu\n", sizeof(long));
  while ((off_t)((void *)ar_hd - *mfile) < pinfo->fsize)
  {
    if (!(name = get_ar_name(ar_hd, &extended)))
      return ;

  //  printf("%s\n", name);

  //  printf("%lld\n", ft_atoll(ar_hd->ar_size));
  //  printf("%x\n", *(int *)((void *)ar_hd + sizeof(struct ar_hdr) + (extended ? ft_atoll(ar_hd->ar_name + ft_strlen(AR_EFMT1)) : 0)));
    if (extended && (!ft_strcmp(name, SYMDEF) || !ft_strcmp(name, SYMDEF_SORTED)))
    {
      ;
      //  ar_hd = (void *)ar_hd + sizeof(struct ar_hdr) + (extended ? ft_atoll(ar_hd->ar_name + ft_strlen(AR_EFMT1)) : 0);
      //  ar_hd = (struct ar_hdr *)((void *)ar_hd + *(int *)ar_hd + sizeof(int));
  //      printf("%x\n", *(int *)ar_hd);
      //  ar_hd = (struct ar_hdr *)((void *)ar_hd + *(int *)ar_hd + sizeof(int));
      //  ar_hd = (struct ar_hdr *)((void *)ar_hd + ft_atoll(ar_hd->ar_size) + sizeof(struct ar_hdr));
        //printf("name %s\n",name);
    }
    else
    {
      // if (extended)
      // {
      //   ar_hd = ar_hd = (struct ar_hdr *)((void *)ar_hd + );
      // }
    //  printf("%s\n", name);

      hd = (struct mach_header_64 *)((void *)ar_hd + sizeof(struct ar_hdr) + (extended ? ft_atoll(ar_hd->ar_name + ft_strlen(AR_EFMT1)) : 0));
      fpinfo = get_parse_info((void *)hd);
      fpinfo.fsize = ft_atoll(ar_hd->ar_size);
      fpinfo.file_name = pinfo->file_name;
      fpinfo.options = pinfo->options;
      fpinfo.ar_from = name;
      fpinfo.fat_arch_from = pinfo->fat_arch_from;
      fsize = fpinfo.fsize;
      if (fpinfo.arch != 32 && fpinfo.arch != 64)
          ;//printf("arch in file chelou magic %d\n", hd->magic);
      else
      {
        if (fpinfo.file_type == MH_FILE)
        {
          if (!(((t_nm_options *)pinfo->options)->flags & OPT_O))
            printf("\n%s(%s)", pinfo->file_name, name);
          if (pinfo->fat_arch_from && !(((t_nm_options *)pinfo->options)->flags & OPT_O))
            printf(" (for architecture %s):\n", pinfo->fat_arch_from);
          else if (!(((t_nm_options *)pinfo->options)->flags & OPT_O))
            printf(":\n");

          mcurfile = (void *)hd;

          //; printf("HFLUzefzefzefzef\n");
          // printf("%d %d\n", fpinfo.endian, fpinfo.arch);
          handle_macho_file(&mcurfile, &fpinfo, 0);
          // update_fat_symtab(pinfo, &fpinfo);
        }
        else
          ;//printf("magic %d in FAT file\n", hd->magic);
      }

    }
    ar_hd = (struct ar_hdr *)((void *)ar_hd + ft_atoll(ar_hd->ar_size) + sizeof(struct ar_hdr));
    free(name);
  }
  //handle_macho_file(&mcurfile, (void *)hd, &fpinfo, options);
//   (void)mfile;
//   (void)pinfo;
//   (void)options;
// //  printf("SALUT\n");
}
