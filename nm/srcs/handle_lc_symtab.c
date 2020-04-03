#include "ft_nm.h"

void handle_lc_symtab(void *addr, t_pinfo *pinfo, void *filestart)
{
  struct symtab_command *symtab_cmd;
  void                  *symtab;
  void                  *strtab_addr;
  uint32_t              nsyms;
  char                  *symname;
  // struct section_64     *section;

  // printf("%zu, %zu\n", sizeof(ab), sizeof(unsigned long));
  symtab_cmd = (struct symtab_command *)addr;
  symtab = filestart + pinfo->get_uint32_t(symtab_cmd->symoff);
  strtab_addr = filestart + pinfo->get_uint32_t(symtab_cmd->stroff);
  nsyms = pinfo->get_uint32_t(symtab_cmd->nsyms);
  update_symtab_size(pinfo, nsyms);
  //printf("nb syms %u symoff %x stroff %x\n", nsyms, pinfo->get_uint32_t(symtab_cmd->symoff), symtab_cmd->stroff);
  while (nsyms--)
  {
    //printf("%u\n", ((struct nlist *)(symtab))->n_un.n_strx);
    if (pinfo->arch == 32)
    {
      symname = (char *)(strtab_addr + pinfo->get_uint32_t(((struct nlist *)(symtab))->n_un.n_strx));
      pinfo->symtab[pinfo->symid++] = (t_symtab){symtab, symname};
  //    printf("nsect %u n_type %x n_type & N_TYPE %x %x\n", ((struct nlist *)(symtab))->n_sect, ((struct nlist *)(symtab))->n_type, ((struct nlist *)(symtab))->n_type & N_TYPE, ((struct nlist *)(symtab))->n_value);
      symtab += sizeof(struct nlist);
    }
    if (pinfo->arch == 64)
    {
      symname = (char *)(strtab_addr + pinfo->get_uint32_t(((struct nlist_64 *)(symtab))->n_un.n_strx));
      pinfo->symtab[pinfo->symid++] = (t_symtab){symtab, symname};
      //printf("nsect %u n_type %x n_type & N_TYPE %x %llx\n", ((struct nlist_64 *)(symtab))->n_sect, ((struct nlist_64 *)(symtab))->n_type, ((struct nlist_64 *)(symtab))->n_type & N_TYPE, ((struct nlist_64 *)(symtab))->n_value);
      symtab += sizeof(struct nlist_64);
    }
  //  printf("sym name %s\n", symname);
  }
  // while (nsects--)
  // {
  //     printf("secname %s in seg %s\n", section->sectname, section->segname);
  //     section = (void *)section + sizeof(struct section_64);
  // }
  // printf("segname %s vm addr %llx nb sections %u file off %llu\n", symtab->segname, symtab->vmaddr, symtab->nsects, symtab->fileoff);
  // (void)pinfo;
}
