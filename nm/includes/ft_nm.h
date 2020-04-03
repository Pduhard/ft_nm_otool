#ifndef FT_NM_H
# define FT_NM_H

# include "../../libft/libft.h"
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>

/*    OPTIONS define  */

# define OPT_A  1

/*                  */

# define ARCH_32  32
# define ARCH_64  64

typedef struct s_symtab
{
  void         *symaddr;
  char         *name;
}              t_symtab;

typedef struct s_sectab
{
  void         *secaddr;
}              t_sectab;

typedef struct s_pinfo
{
  uint32_t  (*get_uint32_t)(uint32_t);
  t_sectab  *sectab;
  t_symtab  *symtab;
  uint32_t  secid;
  uint32_t  symid;
  int   endian; //same 0 other 1
  int   arch;   //32x - 64x
}             t_pinfo;

/*  conversions functions */

uint32_t  reverse_uint32_t(uint32_t nb);
uint32_t  same_uint32_t(uint32_t nb);

/*  command handle functions  */
void handle_load_command(struct load_command *load_command, t_pinfo *pinfo, void *filestart);

void handle_lc_segment_64(void *addr, t_pinfo *pinfo);
void handle_lc_segment(void *addr, t_pinfo *pinfo);
void handle_lc_symtab(void *addr, t_pinfo *pinfo, void *filestart);

/*  utils functions */

void update_symtab_size(t_pinfo *pinfo, uint32_t addsize);
void update_sectab_size(t_pinfo *pinfo, uint32_t addsize);

/*  sort functions (maybe up section) */

void sort_symtab(t_pinfo *pinfo);

/*        */

void  assign_symbol(t_pinfo *pinfo, uint32_t options);

/*        */

void     display_file_sym(void *mfile, uint32_t options, char *file_name);

#endif
