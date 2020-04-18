#include "common.h"

int  check_load_command(struct load_command *load_command, t_pinfo *pinfo, uint32_t load_cmd_id)
{
  uint32_t cmd;

  cmd = pinfo->get_uint32_t(load_command->cmd);
  if (cmd == LC_SYMTAB && !check_lc_symtab(load_command, pinfo, load_cmd_id))
    return (0);
  if (cmd == LC_SEGMENT && !check_lc_segment(load_command, pinfo, load_cmd_id))
    return (0);
  if (cmd == LC_SEGMENT_64 && !check_lc_segment_64(load_command, pinfo, load_cmd_id))
    return (0);
  else if (cmd != LC_SEGMENT && cmd != LC_SYMTAB	&& cmd != LC_SYMSEG
    && cmd != LC_THREAD	&& cmd != LC_UNIXTHREAD && cmd !=	LC_LOADFVMLIB
    && cmd !=	LC_IDFVMLIB	&& cmd !=	LC_IDENT && cmd != LC_FVMFILE && cmd != LC_PREPAGE
    && cmd != LC_DYSYMTAB && cmd != LC_LOAD_DYLIB && cmd != LC_ID_DYLIB && cmd != LC_LOAD_DYLINKER
    && cmd != LC_ID_DYLINKER && cmd != LC_PREBOUND_DYLIB && cmd != LC_ROUTINES && cmd != LC_SUB_FRAMEWORK
    && cmd != LC_SUB_UMBRELLA && cmd != LC_SUB_CLIENT && cmd != LC_SUB_LIBRARY && cmd != LC_TWOLEVEL_HINTS
    && cmd != LC_PREBIND_CKSUM && cmd != LC_LOAD_WEAK_DYLIB && cmd != LC_SEGMENT_64 && cmd != LC_ROUTINES_64
    && cmd != LC_UUID && cmd != LC_RPATH && cmd != LC_CODE_SIGNATURE && cmd != LC_SEGMENT_SPLIT_INFO
    && cmd != LC_REEXPORT_DYLIB && cmd != LC_LAZY_LOAD_DYLIB && cmd != LC_ENCRYPTION_INFO
    && cmd != LC_DYLD_INFO && cmd != LC_DYLD_INFO_ONLY && cmd != LC_LOAD_UPWARD_DYLIB
    && cmd != LC_VERSION_MIN_MACOSX && cmd != LC_VERSION_MIN_IPHONEOS && cmd != LC_FUNCTION_STARTS
    && cmd != LC_DYLD_ENVIRONMENT && cmd != LC_MAIN && cmd != LC_DATA_IN_CODE && cmd != LC_SOURCE_VERSION
    && cmd != LC_DYLIB_CODE_SIGN_DRS && cmd != LC_ENCRYPTION_INFO_64 && cmd != LC_LINKER_OPTION
    && cmd != LC_LINKER_OPTIMIZATION_HINT)
  {
    ft_fdprintf(2, "%s malformed object (unknown load command %u)\n", pinfo->file_name, load_cmd_id);
    return (0);
  }
  return (1);
}

void handle_load_command(struct load_command *load_command, t_pinfo *pinfo, void *filestart)
{
  uint32_t cmd;

  cmd = pinfo->get_uint32_t(load_command->cmd);
  if (cmd == LC_SEGMENT)
    handle_lc_segment(((void *)load_command), pinfo);
  else if (cmd == LC_SEGMENT_64)
    handle_lc_segment_64(((void *)load_command), pinfo);
  else if (cmd == LC_SYMTAB && pinfo->bin == BIN_NM)
    handle_lc_symtab(((void *)load_command), pinfo, filestart);
}
