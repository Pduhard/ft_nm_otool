#include "ft_nm.h"

void handle_lc_segment(void *addr, t_pinfo *pinfo)
{
  struct segment_command *segment;
  uint32_t               nsects;
  struct section         *section;

  segment = (struct segment_command *)addr;
  section = (struct section *)(addr + sizeof(struct segment_command));
  nsects = pinfo->get_uint32_t(segment->nsects);
  update_sectab_size(pinfo, nsects);
  while (nsects--)
  {
    //  printf("secname %s in seg %s adddr %llx\n", section->sectname, section->segname, section->addr);
      pinfo->sectab[pinfo->secid++] = (t_sectab){(void *)section};
      section = (void *)section + sizeof(struct section);
  }
//  printf("secid %d\n", pinfo->secid);
//  printf("segname %s vm addr %llx nb sections %u file off %llu\n", segment->segname, segment->vmaddr, segment->nsects, segment->fileoff);
  (void)pinfo;
}
