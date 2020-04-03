#include "ft_nm.h"

void handle_load_command(struct load_command *load_command, t_pinfo *pinfo, void *filestart)
{
  if (load_command->cmd == LC_SEGMENT)
    handle_lc_segment(((void *)load_command), pinfo);
  if (load_command->cmd == LC_SEGMENT_64)
    handle_lc_segment_64(((void *)load_command), pinfo);
  if (load_command->cmd == LC_SYMTAB)
    handle_lc_symtab(((void *)load_command), pinfo, filestart);
}
