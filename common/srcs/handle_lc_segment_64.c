#include "common.h"

int  check_lc_segment_64(struct load_command *load_command, t_pinfo *pinfo, uint32_t load_cmd_id)
{
  struct segment_command_64 *segment;
  uint32_t                  nsects;
  struct section_64         *section;

  segment = (struct segment_command_64 *)load_command;
  if (pinfo->get_uint32_t(load_command->cmdsize) != pinfo->get_uint32_t(segment->nsects) * sizeof(struct section_64) + sizeof(struct segment_command_64))
  {
    ft_fdprintf(2, "malformed object (inconsistent cmdsize in LC_SEGMENT_64 command %u for the number of sections)\n", load_cmd_id);
    return (0);
  }
  if (pinfo->get_uint64_t(segment->fileoff) + pinfo->get_uint64_t(segment->filesize) > (uint64_t)pinfo->fsize)
  {
    ft_fdprintf(2, "truncated or malformed object (offset plus size in LC_SEGMENT_64 command %u extends past the end of the file)\n", load_cmd_id);
    return (0);
  }
  nsects = pinfo->get_uint32_t(segment->nsects);
  section = (struct section_64 *)(segment + 1);
  // while (nsects--)
  // {
  //   //  printf("secname %s in seg %s adddr %llx\n", section->sectname, section->segname, section->addr);
  //     if ((off_t)(pinfo->get_uint32_t(section->offset) + pinfo->get_uint64_t(section->size)) > pinfo->fsize)
  //     {
  //       ft_fdprintf(2, "truncated or malformed object (offset plus size of a section in LC_SEGMENT_64 command %u extends past the end of the file)\n", load_cmd_id);
  //       return (0);
  //     }
  //     section++;
  // }
  return (1);
}

void handle_lc_segment_64(void *addr, t_pinfo *pinfo)
{
  struct segment_command_64 *segment;
  uint32_t                  nsects;
  struct section_64         *section;

  segment = (struct segment_command_64 *)addr;
  section = (struct section_64 *)(addr + sizeof(struct segment_command_64));
  nsects = pinfo->get_uint32_t(segment->nsects);
  update_sectab_size(pinfo, nsects);
  while (nsects--)
  {
      // printf("secname %s in seg %s adddr %llx\n", section->sectname, section->segname, section->addr);
      pinfo->sectab[pinfo->secid++] = (t_sectab){(void *)section};
      section = (void *)section + sizeof(struct section_64);
  }
//  printf("secid %d\n", pinfo->secid);
//  printf("segname %s vm addr %llx nb sections %u file off %llu\n", segment->segname, segment->vmaddr, segment->nsects, segment->fileoff);
  (void)pinfo;
}
