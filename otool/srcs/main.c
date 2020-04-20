#include "ft_otool.h"

void print_memory(t_pinfo *pinfo, uint64_t addr, uint32_t offset, uint64_t size)
{
  unsigned char *mem;
  uint32_t      i;

 // printf("%llx %llx %x %llx %llx\n", (unsigned long long)pinfo->fsize, addr, offset, addr - offset, size);
  mem = (unsigned char *)(pinfo->file_start + offset);
  while ((long long)size > 0 && (off_t)((void *)mem - pinfo->file_start) < pinfo->fsize)
  {
    printf(pinfo->arch == ARCH_32 ? "%08llx " : "%016llx ", addr);
    i = 0;
    while (i < 16 && i < size && (off_t)((void *)mem - pinfo->file_start + i) < pinfo->fsize)
    {
      printf("%02x ", mem[i++]);
    }
    mem += 16;
    addr += 16;
    printf("\n");
    size -= 16;
  }
}

void print_text_section(t_pinfo *pinfo, t_sectab *sectab)
{
  struct section    *sect;
  struct section_64 *sect_64;
  uint64_t          size;
  uint32_t          sect_off;
  uint64_t          sect_size;

  sect = NULL;
  sect_64 = NULL;
  if (pinfo->arch == ARCH_32)
    sect = (struct section *)sectab->secaddr;
  else
    sect_64 = (struct section_64 *)sectab->secaddr;
  sect_off = sect ? pinfo->get_uint32_t(sect->offset) : pinfo->get_uint32_t(sect_64->offset);
  sect_size = sect ? pinfo->get_uint32_t(sect->size) : pinfo->get_uint64_t(sect_64->size);
  if ((void *)sect + sect_size > pinfo->file_start + pinfo->fsize)
  {

    // sect_size = pinfo->file_start + pinfo->fsize - (void *)sect;
    //
    // printf("azdazd\n");
  }
  if ((off_t)(sect_off + sect_size) > pinfo->fsize)
  {
    size = pinfo->arch == ARCH_32 ? pinfo->fsize - sect_off : 0;
    // size = sect_size;
    // printf("%llx %x\n", size);
    printf("section (__TEXT,__text) extends past end of file\n");
  }
  else
    size = sect_size;
  printf("(%s,%s) section\n", SEG_TEXT, SECT_TEXT);

//  printf("%s %s\n", sect->segname, sect->sectname);
  print_memory(pinfo, sect ? pinfo->get_uint32_t(sect->addr) : pinfo->get_uint64_t(sect_64->addr), sect_off, size);
  // printf("section text \n");
  (void)pinfo;
  (void)sectab;
}

void print_otool(t_pinfo *pinfo, int display)
{
  uint32_t  i;
  char      segname[16];
  char      sectname[16];

  i = 0;
  // name = get_ar_name(ar_hd, &extended)
  if (!pinfo->ar_from)
    printf("%s:\n", pinfo->file_name);
  else
    printf("%s(%s):\n", pinfo->file_name, pinfo->ar_from);
// (__TEXT,__text) section
  while (i < pinfo->secid)
  {
    if (get_sect_names(segname, sectname, pinfo, i))
    {
      // printf("?? %s %s\n", segname, sectname);
      // printf("%S")
      if (/* OPTION_T */ !ft_strncmp(segname, SEG_TEXT, sizeof(SEG_TEXT))
        && !ft_strncmp(sectname, SECT_TEXT, sizeof(SECT_TEXT)))
          print_text_section(pinfo, pinfo->sectab + i - 1);
    }
    i++;
  }
  //printf("OK otool\n");
    (void)pinfo;
    (void)display;
}

int main(int argc, char **argv)
{
  off_t     file_size;
  t_pinfo   pinfo;
  void      *mfile;
  uint32_t  i;

  i = 1;
  (void)argc;
  // ft_putendl(argv[argc - 1]);
  while (argv[i])
  {
    if ((mfile = mapp_file(argv[i], argv[0], &file_size)))
    {
      pinfo = get_parse_info(mfile, BIN_OTOOL);
      pinfo.file_name = argv[i];
      pinfo.fsize = file_size;
      pinfo.print = &print_otool;
      //pinfo.options = (void *)&opt;
      // printf("%d\n", pinfo.file_type);
      if (pinfo.arch == 32 || pinfo.arch == 64)
        handle_file(mfile, &pinfo, argv[1] ? DISPLAY_INFO_ON : DISPLAY_INFO_OFF);
      else
        printf("%s: is not an object file\n", pinfo.file_name);
    }
    i++;
  }
  return (0);
  // if (ft_strcmp(argv[0], "./ft_otool"))
  //   handle_lc_symtab(NULL, NULL, NULL);
  // get_symbol(0, 0, 0, NULL);
  return (0);
}
