#include "ft_otool.h"

void print_memory(t_pinfo *pinfo, uint64_t addr, uint32_t offset, uint64_t size, char display)
{
  unsigned char *mem;
  uint32_t      i;

 // printf("%llx %llx %x %llx %llx\n", (unsigned long long)pinfo->fsize, addr, offset, addr - offset, size);

     // printf("cputype %x (i386: %x MC680x0 %x)\n", pinfo->cputype, CPU_TYPE_I386, CPU_TYPE_MC680x0);
  mem = (unsigned char *)(pinfo->file_start + offset);
  while ((long long)size > 0 && (off_t)((void *)mem - pinfo->file_start) < pinfo->fsize)
  {
    printf(pinfo->arch == ARCH_32 ? "%08llx%c" : "%016llx%c", addr, display);
    i = 0;

    while (i < 16 && i < size && (off_t)((void *)mem - pinfo->file_start + i) < pinfo->fsize)
    {
      if ((pinfo->cputype & ~CPU_ARCH_MASK) == CPU_TYPE_I386)
        printf("%02x ", mem[i++]);
      else if ((pinfo->cputype & ~CPU_ARCH_MASK) == CPU_TYPE_MC680x0)
      {
        printf("%04x ", pinfo->get_uint16_t(*(uint16_t *)(mem + i)));
        i += 2;
      }
      else
      {
        printf("%08x ", pinfo->get_uint32_t(*(uint32_t *)(mem + i)));
        i += 4;
      }
    }
    mem += 16;
    addr += 16;
    printf("\n");
    size -= 16;
  }
}

void print_section(t_pinfo *pinfo, t_sectab *sectab, char *segname, char *sectname, char display)
{
  struct section    *sect;
  struct section_64 *sect_64;
  uint64_t          size;
  uint32_t          sect_off;
  uint64_t          sect_size;

  sect = NULL;
  sect_64 = NULL;
  // printf("ah\n");
  if (pinfo->arch == ARCH_32)
    sect = (struct section *)sectab->secaddr;
  else
    sect_64 = (struct section_64 *)sectab->secaddr;
  // printf("%p %u\n", sect, *(unsigned int*)sect_64);
  sect_off = sect ? pinfo->get_uint32_t(sect->offset) : pinfo->get_uint32_t(sect_64->offset);
  sect_size = sect ? pinfo->get_uint32_t(sect->size) : pinfo->get_uint64_t(sect_64->size);

  // printf("AH\n");
  if ((void *)sect + sect_size > pinfo->file_start + pinfo->fsize)
  {

    // sect_size = pinfo->file_start + pinfo->fsize - (void *)sect;
    //
    // printf("azdazd\n");
  }
   // printf("%u %llu %llu\n", sect_off, sect_size, pinfo->fsize);
  if ((off_t)sect_off > pinfo->fsize)
  {
    size = 0;
    printf("section offset for section (%s,%s) is past end of file\n", segname, sectname);
  }
  else if (sect_size > (unsigned long long)pinfo->fsize || (off_t)(sect_off + sect_size) > pinfo->fsize)
  {
    size = pinfo->arch == ARCH_32 ? pinfo->fsize - sect_off : 0; // CHELOU ouai
    // size = sect_size;
    // printf("%llx %x\n", size);
    printf("section (%s,%s) extends past end of file\n", segname, sectname);
  }
  else
    size = sect_size;
  printf("(%s,%s) section\n", segname, sectname);

//  printf("%s %s\n", sect->segname, sect->sectname);
  print_memory(pinfo, sect ? pinfo->get_uint32_t(sect->addr) : pinfo->get_uint64_t(sect_64->addr), sect_off, size, display);
  // printf("section text \n");
  (void)pinfo;
  (void)sectab;
}

int check_otool_seg(t_pinfo *pinfo, struct segment_command *seg, uint32_t sizeofcmds, uint32_t *secid, char *segname, char *sectname, char display)
{
    struct section *sect;
    uint32_t  i;
    // uint32_t  new_id;

    i = 0;
    // sect = pinfo->file_start + pinfo->get_uint32_t(seg->fileoff);
    sect = (struct section *)((void *)seg + sizeof(struct segment_command));
    // new_id = *secid + pinfo->get_uint32_t(seg->nsects);
    while (i < pinfo->get_uint32_t(seg->nsects))
    {

      // printf("%.6s %.6s\n", sect->segname, sect->sectname);
      if ((void *)sect + sizeof(struct section) > (void *)pinfo->loadc + sizeofcmds)
      {
        // *secid = new_id;
        // return (0);
        printf("section structure command extends past end of load commands\n");
        return (0);
      }
      if (/* OPTION_T */ !ft_strncmp(sect->segname, segname, ft_strlen(segname))
        && !ft_strncmp(sect->sectname, sectname, ft_strlen(sectname)))
        {
          // printf("secid %d pinfoid %u\n", *secid, pinfo->secid);
          print_section(pinfo, pinfo->sectab + *secid, segname, sectname, display);
          return (1);
          // check = 1;
        }

      sect++;
      // = (void *)sect + sizeof(struct section);
      i++;
      (*secid)++;
    }
    return (0);


}

int check_otool_seg_64(t_pinfo *pinfo, struct segment_command_64 *seg_64, uint32_t sizeofcmds, uint32_t *secid, char *segname, char *sectname, char display)
{
    struct section_64 *sect_64;
    uint32_t  i;
    uint32_t  new_id;

    i = 0;
    // sect = pinfo->file_start + pinfo->get_uint32_t(seg->fileoff);
    sect_64 = (struct section_64 *)((void *)seg_64 + sizeof(struct segment_command_64));
    new_id = *secid + seg_64->nsects;
    while (i < pinfo->get_uint32_t(seg_64->nsects))
    {
      if ((void *)sect_64 + sizeof(struct section_64) > (void *)pinfo->loadc + sizeofcmds)
      {
        // *secid = new_id;
        // return (0);
        printf("section structure command extends past end of load commands\n");
        return (0);
      }
      if (/* OPTION_T */ !ft_strncmp(sect_64->segname, segname, ft_strlen(segname))
        && !ft_strncmp(sect_64->sectname, sectname, ft_strlen(sectname)))
        {
          print_section(pinfo, pinfo->sectab + *secid, segname, sectname, display);
          return (1);
          // check = 1;
        }
      sect_64++;
      i++;
      (*secid)++;
    }

    return (0);
}

int print_otool_section(t_pinfo *pinfo, char *segname, char *sectname, char display)
{
  uint32_t  ncmds;
  uint32_t  i;
  struct load_command *load_c;
  struct segment_command  *seg;
  struct segment_command_64 *seg_64;
  uint32_t  sizeofcmds;
  uint32_t  tt_size;
  uint32_t  j;
  uint32_t  secid;

  secid = 0;

  // printf("%u\n", 45);
  ncmds = get_number_load_command((void *)pinfo->loadc - (pinfo->arch == ARCH_32 ? sizeof(struct mach_header) : sizeof(struct mach_header_64)), pinfo);
  if (pinfo->arch == 64)
    sizeofcmds = pinfo->get_uint32_t(((struct mach_header_64 *)((void *)pinfo->loadc - sizeof(struct mach_header_64)))->sizeofcmds);
  else
    sizeofcmds = pinfo->get_uint32_t(((struct mach_header *)((void *)pinfo->loadc - sizeof(struct mach_header)))->sizeofcmds);

    load_c = pinfo->loadc;
  if ((off_t)((void *)load_c + sizeofcmds) > pinfo->fsize)
    sizeofcmds = pinfo->fsize - (off_t)((void *)load_c - pinfo->file_start);
  // printf("%d\n", sizeofcmds);
  // sizeofcmds =
  i = 0;
  tt_size = 0;
  while (i < ncmds && tt_size < sizeofcmds)
  {


      // printf("é\n");
    j = 0;
    seg = NULL;
    seg_64 = NULL;
    if (pinfo->get_uint32_t(load_c->cmdsize) % sizeof(uint32_t))
        printf("load command %u size not a multiple of sizeof(int32_t)\n", i);
     // printf("%u += %u\n", tt_size, pinfo->get_uint32_t(load_c->cmdsize));
    if (!pinfo->get_uint32_t(load_c->cmdsize))
    {
      printf("load command %u size zero (can't advance to other load commands)\n", i);
      return (0);
    }
    tt_size += pinfo->get_uint32_t(load_c->cmdsize);// + pinfo->get_uint32_t(load_c->cmdsize) % sizeof(uint32_t);
    if (tt_size > sizeofcmds)
    {
      printf("load command %u extends past end of load commands\n", i);
      // return (0);
    }
    // if ()
    if (pinfo->get_uint32_t(load_c->cmd) == LC_SEGMENT)
      seg = (struct segment_command *)load_c;
    else if (pinfo->get_uint32_t(load_c->cmd) == LC_SEGMENT_64)
      seg_64 = (struct segment_command_64 *)load_c;
    if (seg && check_otool_seg(pinfo, seg, sizeofcmds, &secid, segname, sectname, display))
      return (1);
    else if (seg_64 && check_otool_seg_64(pinfo, seg_64, sizeofcmds, &secid, segname, sectname, display))
      return (1);
    // printf("a\n");

    load_c = (void *)load_c + pinfo->get_uint32_t(load_c->cmdsize);//m + pinfo->get_uint32_t(load_c->cmdsize) % sizeof(uint32_t);

      // printf'
      // printf("")
    i++;
  }
  return (0);
}

void print_otool(t_pinfo *pinfo, int display)
{
  uint32_t  i;
  // char      segname[16];
  // char      sectname[16];
  // int       check = 0;

  i = 0;
  // name = get_ar_name(ar_hd, &extended)
  if (pinfo->fat_arch_from)
    printf("%s (architecture %s):\n", pinfo->file_name, pinfo->fat_arch_from);
  else if (!pinfo->ar_from)
    printf("%s:\n", pinfo->file_name);
  else
    printf("%s(%s):\n", pinfo->file_name, pinfo->ar_from);
    // printf("bin\n");
  if ((pinfo->options->flags & OPT_OTOOL_T) && !print_otool_section(pinfo, SEG_TEXT, SECT_TEXT, ' '))
      printf("(%s,%s) section\n", SEG_TEXT, SECT_TEXT);
  if ((pinfo->options->flags & OPT_OTOOL_D))
      print_otool_section(pinfo, SEG_DATA, SECT_DATA, '\t');
      // printf("(%s,%s) section\n", SEG_TEXT, SECT_TEXT);
// (__TEXT,__text) section

      // printf("HA\n");
      // printf("XD\n");

  // while (i < pinfo->secid)
  // {
  //
  //   if (get_sect_names(segname, sectname, pinfo, i + 1))
  //   {
  //         // printf("HA\n");
  //     // printf("?? %s %s\n", segname, sectname);
  //     // printf("%S")
  //     if (/* OPTION_T */ !ft_strncmp(segname, SEG_TEXT, sizeof(SEG_TEXT))
  //       && !ft_strncmp(sectname, SECT_TEXT, sizeof(SECT_TEXT)))
  //       {
  //         print_text_section(pinfo, pinfo->sectab + i);
  //         check = 1;
  //       }
  //   }
  //   i++;
  // }
// if (!check/* && OPTION_T */)
// printf("(%s,%s) section\n", SEG_TEXT, SECT_TEXT);
  //printf("OK otool\n");
    (void)pinfo;
    (void)display;
}

uint32_t  check_otool_flag(char **argv, uint32_t *i, t_option *options)
{
    // t_option  *options;
    char          *flag_str;
    int           flag_i;
    uint32_t      opt_flag;

    flag_str = &(argv[*i][1]);
    // options = (t_option *)opt_struct;
    opt_flag = 0;
  //  printf("nm _flag %s\n", flag_str);
    if (!ft_strcmp(flag_str, "arch"))
      return (update_arch_flags(&options->arch_flags, argv[++(*i)], argv[0], OPT_OTOOL_ARCH));
    // if (!ft_strcmp(flag_str, "t"))
    //   return (update_format(&options->format, argv[++(*i)], argv[0]));
    while (*flag_str)
    {
      if ((flag_i = ft_strichr(VALID_OTOOL_FLAGS, *flag_str)) == -1)
        return (ft_return_error(OPT_ERROR, "error: %s: invalid argument -%c\n", argv[0], *flag_str));
      opt_flag |= (1 << flag_i);
      flag_str++;
    }
    if ((opt_flag & OPT_NM_S))
    {
      if (options->segname || options->sectname)
        return (ft_return_error(OPT_ERROR, "error: %s: more than one -s option specified\n", argv[0], *flag_str));
      if (!argv[++(*i)])
      {
        if (!(options->segname = ft_strdup(""))
          || !(options->sectname = ft_strdup("")))
          return (ft_memalloc_error(OPT_ERROR, 1, argv[0]));
      }
      else
      {
        if (!argv[*i + 1])
          return (ft_return_error(OPT_ERROR, "error: %s: missing arguments to -s\n", argv[0], *flag_str));
        if (!(options->segname = ft_strdup(argv[*i]))
          || !(options->sectname = ft_strdup(argv[*i + 1])))
          return (ft_memalloc_error(OPT_ERROR, 1, argv[0]));
        ++(*i);
      }
    }
    // if (((options->flags | opt_flag) & OPT_NM_MAJ_U) && ((options->flags | opt_flag ) & OPT_NM_U))
    //   return (ft_return_error(OPT_ERROR, "error: %s: can't specifiy both -U and -u\n", argv[0]));
    // if (opt_flag & OPT_NM_MAJ_A)
    //   opt_flag |= OPT_NM_O;
    return (opt_flag);
}

int main(int argc, char **argv)
{
  off_t     file_size;
  char      **files;
  t_pinfo   pinfo;
  void      *mfile;
  uint32_t  i;
  t_option  opt;

  ft_bzero(&opt, sizeof(t_option));
  i = 0;
  if (argc < 2)
    return (ft_usage_error(1, OTOOL_USAGE, argv[0]));
  if (!(files = parse_arguments(argv, &opt, &check_otool_flag, &opt.flags)))
  {
    ft_throw_error("error: %s: at least one file must be specified\n", argv[0]);
    return (ft_usage_error(1, OTOOL_USAGE, argv[0]));
  }
  if (!(opt.flags & ~OPT_OTOOL_ARCH))
  {
      ft_throw_error("error: %s: one of -fahltd[s segname sectname] must be specified\n", argv[0]);
      return (ft_usage_error(1, OTOOL_USAGE, argv[0]));
  }
  if ((opt.flags & OPT_OTOOL_ARCH) && !check_arch_flags(&opt))
    return (ft_usage_error(1, OTOOL_USAGE, argv[0]));

  (void)argc;
  // ft_putendl(argv[argc - 1]);
  while (files[i])
  {
    if ((mfile = mapp_file(files[i], argv[0], &file_size, BIN_OTOOL)))
    {
      if (file_size < (off_t)sizeof(struct mach_header) && (*(uint32_t *)mfile == MH_MAGIC || *(uint32_t *)mfile == MH_CIGAM || *(uint32_t *)mfile == MH_MAGIC_64 || *(uint32_t *)mfile == MH_CIGAM_64))
        printf("%s: is not an object file\n", files[i]);
      else
      {
        pinfo = get_parse_info(mfile, BIN_OTOOL);
        pinfo.file_name = files[i];
        pinfo.fsize = file_size;
        pinfo.print = &print_otool;
        pinfo.options = &opt;
      //pinfo.options = (void *)&opt;
      // printf("%d\n", pinfo.file_type);
        if (pinfo.arch == 32 || pinfo.arch == 64)
          handle_file(mfile, &pinfo, files[1] ? DISPLAY_INFO_ON : DISPLAY_INFO_OFF);
        else
          printf("%s: is not an object file\n", pinfo.file_name);
      }
    }
    i++;
  }
  return (0);
  // if (ft_strcmp(argv[0], "./ft_otool"))
  //   handle_lc_symtab(NULL, NULL, NULL);
  // get_symbol(0, 0, 0, NULL);
  return (0);
}
