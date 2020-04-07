#ifndef FT_NM_H
# define FT_NM_H

# include "../../libft/libft.h"
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach-o/arch.h>
# include <mach-o/ranlib.h>
# include <ar.h>

/*    OPTIONS define  */

# define OPT_A    0b1
# define OPT_ARCH 0b10

# define FAT_ARCH_ALL 0
# define FAT_ARCH_SPEC 1

/*                  */

# define ARCH_32  32
# define ARCH_64  64
// # define BIG_ENDIAN 0
// # define LITTLE_ENDIAN 1

typedef struct s_symtab
{
  //void         *symaddr;
  struct nlist_64 sym;
  char          symbol;
  char         *name;
  char         *indr;
}              t_symtab;

typedef struct s_sectab
{
  void         *secaddr;
}              t_sectab;

typedef enum
{
  MH_FILE,
  FAT_FILE,
  ARCHIVE_FILE,
  UNKNOWN_FILE,
} e_file_type;

typedef enum
{
  _ARCH_32,
  _ARCH_64,
} e_arch_type;

typedef enum
{
  BIG,
  LITTLE,
} e_endian_type;

typedef struct s_cpu_info
{
  e_endian_type endian;
  e_arch_type   arch;
}               t_cpu_info;

typedef struct s_pinfo
{
  uint32_t  (*get_uint32_t)(uint32_t);
  uint64_t  (*get_uint64_t)(uint64_t);
  const NXArchInfo  *myinfo;
  t_sectab  *sectab;
  t_symtab  *symtab;
  uint32_t  secid;
  uint32_t  symid;
  int   endian; //same 0 other 1
  int   arch;   //32x - 64x
  e_file_type   file_type; //
  char        *file_name;
  off_t       fsize;
}             t_pinfo;

/*  conversions functions */

uint32_t  reverse_uint32_t(uint32_t nb);
uint32_t  same_uint32_t(uint32_t nb);
uint64_t  reverse_uint64_t(uint64_t nb);
uint64_t  same_uint64_t(uint64_t nb);

/*  file type handling functions */
t_pinfo      get_parse_info(void *mfile);

void  handle_macho_file(void **mfile, t_pinfo *pinfo, uint32_t options);
void  handle_fat_file(void **mfile, t_pinfo *pinfo, uint32_t options);
void  handle_archive_file(void **mfile, t_pinfo *pinfo, uint32_t options);

/* check functions */

int  check_load_command(struct load_command *load_command, t_pinfo *pinfo, uint32_t load_cmd_id);
int  check_lc_symtab(struct load_command *load_command, t_pinfo *pinfo, uint32_t load_cmd_id);
int  check_lc_segment(struct load_command *load_command, t_pinfo *pinfo, uint32_t load_cmd_id);
int  check_lc_segment_64(struct load_command *load_command, t_pinfo *pinfo, uint32_t load_cmd_id);
int  check_macho_file(void *mfile, t_pinfo *pinfo);
int  check_archive_file(void *mfile, t_pinfo *pinfo);

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
char  get_symbol(uint8_t n_type, uint8_t n_sect, uint64_t n_value, t_pinfo *pinfo);
void  assign_symbol(t_pinfo *pinfo, uint32_t options);

/*        */

void     display_file_sym(void *mfile, uint32_t options, char *file_name, off_t fsize);

#endif
