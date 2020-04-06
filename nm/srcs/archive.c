#include "ft_nm.h"

void  handle_archive_file(void **mfile, t_pinfo *pinfo, uint32_t options)
{
  struct ar_hdr *ar_hd;
  void          *mcurfile;
  struct mach_header_64 *hd;
  t_pinfo       fpinfo;
  uint32_t      extended;
  char          *name;

  ar_hd = (struct ar_hdr *)(*mfile + SARMAG);
//  printf("%zu\n", sizeof(long));
  while ((off_t)((void *)ar_hd - *mfile) < pinfo->fsize)
  {
    extended = ft_strncmp(ar_hd->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) ? 0 : 1;
    name = extended ? (char *)(ar_hd + 1) : ar_hd->ar_name;
  //  printf("%s\n", name);

  //  printf("%lld\n", ft_atoll(ar_hd->ar_size));
  //  printf("%x\n", *(int *)((void *)ar_hd + sizeof(struct ar_hdr) + (extended ? ft_atoll(ar_hd->ar_name + ft_strlen(AR_EFMT1)) : 0)));
    if (extended && (!ft_strcmp(name, SYMDEF) || !ft_strcmp(name, SYMDEF_SORTED)))
    {
        ar_hd = (void *)ar_hd + sizeof(struct ar_hdr) + (extended ? ft_atoll(ar_hd->ar_name + ft_strlen(AR_EFMT1)) : 0);
        ar_hd = (struct ar_hdr *)((void *)ar_hd + *(int *)ar_hd
                                    // + sizeof(struct ar_hdr)
                                    // + (extended ? ft_atoll(ar_hd->ar_name + ft_strlen(AR_EFMT1)) : 0))
                                  + sizeof(int));
  //      printf("%x\n", *(int *)ar_hd);
        ar_hd = (struct ar_hdr *)((void *)ar_hd + *(int *)ar_hd + sizeof(int));
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
      if (fpinfo.arch != 32 && fpinfo.arch != 64)
          printf("arch in file chelou magic %d\n", hd->magic);
      else
      {
        if (fpinfo.file_type == MH_FILE)
        {
          printf("\n%s(%s):\n", pinfo->file_name, name);
          mcurfile = (void *)hd;
          // printf("HFLUzefzefzefzef\n");
          // printf("%d %d\n", fpinfo.endian, fpinfo.arch);
          handle_macho_file(&mcurfile, (void *)hd, &fpinfo, options);
          // update_fat_symtab(pinfo, &fpinfo);
        }
        else
          printf("magic %d in FAT file\n", hd->magic);
      }
      ar_hd = (struct ar_hdr *)((void *)ar_hd + ft_atoll(ar_hd->ar_size) + sizeof(struct ar_hdr));

    }
  }
  //handle_macho_file(&mcurfile, (void *)hd, &fpinfo, options);
//   (void)mfile;
//   (void)pinfo;
//   (void)options;
// //  printf("SALUT\n");
}
