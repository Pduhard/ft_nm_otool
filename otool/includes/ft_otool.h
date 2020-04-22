#ifndef FT_OTOOL_H
# define FT_OTOOL_H

# include "../../libft/libft.h"
# include "../../common/includes/common.h"

#define OTOOL_USAGE "[-arch arch_type] [-fahltd[s segname sectname]] <object file> ...\n\t-f print the fat headers\n\t-a print the archive header\n\t-h print the mach header\n\t-l print the load commands\n\t-t print the text section (disassemble with -v)\n\t-s <segname> <sectname> print contents of section\n\t-d print the data section"
#endif
