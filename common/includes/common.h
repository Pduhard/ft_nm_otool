#ifndef COMMON_H
# define COMMON_H

# include "../../libft/libft.h"
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach-o/arch.h>
# include <mach-o/ranlib.h>
# include <mach-o/stab.h>
# include <ar.h>

# define VALID_OTOOL_FLAGS "fahltds"

# define OPT_OTOOL_F    0b1
# define OPT_OTOOL_A    0b10
# define OPT_OTOOL_H    0b100
# define OPT_OTOOL_L    0b1000
# define OPT_OTOOL_T    0b10000
# define OPT_OTOOL_D    0b100000
# define OPT_OTOOL_S    0b1000000
# define OPT_OTOOL_ARCH 0b10000000


# define VALID_NM_FLAGS  "agnopruUmxjlfAPs"
# define OPT_NM_A        0b1
# define OPT_NM_G        0b10
# define OPT_NM_N        0b100
# define OPT_NM_O        0b1000
# define OPT_NM_P        0b10000
# define OPT_NM_R        0b100000
# define OPT_NM_U        0b1000000
# define OPT_NM_MAJ_U    0b10000000
# define OPT_NM_M        0b100000000
# define OPT_NM_X        0b1000000000
# define OPT_NM_J        0b10000000000
# define OPT_NM_L        0b100000000000
# define OPT_NM_F        0b1000000000000
# define OPT_NM_MAJ_A    0b10000000000000
# define OPT_NM_MAJ_P    0b100000000000000
# define OPT_NM_S        0b1000000000000000

# define OPT_NM_T        0b10000000000000000
# define OPT_NM_ARCH     0b100000000000000000

# define NM_DEFAULT_INPUT 0b10000000000000000000
# define OPT_ERROR    0
# define OPT_END          0b1000000000000000000

# define DISPLAY_INFO_ON 1
# define DISPLAY_INFO_OFF 2

# define FAT_ARCH_ALL 4
# define FAT_ARCH_SPEC 8

# define ARCH_32  32
# define ARCH_64  64

typedef struct s_symtab
{
  //void         *symaddr;
  struct nlist_64 sym;
  char            symbol;
  char            *name;
  char            *indr;
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

typedef enum
{
  F_DEC,
  F_OCT,
  F_HEX,
} e_format;

typedef enum
{
  BIN_NM,
  BIN_OTOOL,
} e_bin;

typedef struct s_cpu_info
{
  e_endian_type endian;
  e_arch_type   arch;
}               t_cpu_info;

typedef struct s_option
{
  uint32_t     flags;
  e_format     format;
  char         *segname;
  char         *sectname;
  char         **arch_flags;
}              t_option;

typedef struct s_pinfo
{
  uint16_t  (*get_uint16_t)(uint16_t);
  uint32_t  (*get_uint32_t)(uint32_t);
  uint64_t  (*get_uint64_t)(uint64_t);
  t_option    *options;
  const NXArchInfo  *myinfo;
  t_sectab    *sectab;
  t_symtab    *symtab;
  uint32_t    secid;
  uint32_t    symid;
  int         endian; //same 0 other 1
  int         arch;   //32x - 64x
  e_bin       bin;
  void        (*print)(struct s_pinfo *, int);
  e_file_type file_type; //
  char        *file_name;
  off_t       fsize;
  //struct s_pinfo *from;
  char        *ar_from;
  char        *fat_arch_from;
  int         section_start;
  void        *file_start;
  cpu_type_t  cputype;
  cpu_subtype_t cpusubtype;
  struct load_command * loadc;
}             t_pinfo;



/*  initial parsing functions */

void *mapp_file(char *file_name, char *bin_name, off_t *file_size, e_bin bin);
uint32_t       update_arch_flags(char ***arch_flags_addr, char *new_flag, char *prog_name, uint32_t ret);
uint32_t       update_format(e_format *format, char *new_format, char *prog_name);
char  **parse_arguments(char **argv, t_option *opt_struct,
        uint32_t (*check_valid_flag)(char **, uint32_t *, t_option *), uint32_t *flags);
int       check_arch_flags(t_option *opt);

/*  conversions functions */

uint16_t  reverse_uint16_t(uint16_t nb);
uint16_t  same_uint16_t(uint16_t nb);
uint32_t  reverse_uint32_t(uint32_t nb);
uint32_t  same_uint32_t(uint32_t nb);
uint64_t  reverse_uint64_t(uint64_t nb);
uint64_t  same_uint64_t(uint64_t nb);

/*  file type handling functions */
t_pinfo      get_parse_info(void *mfile, e_bin bin);
void  handle_file(void *mfile, t_pinfo *pinfo, uint32_t display);

void  handle_macho_file(void **mfile, t_pinfo *pinfo, uint32_t display);
void  handle_fat_file(void **mfile, t_pinfo *pinfo, uint32_t display);
void  handle_archive_file(void **mfile, t_pinfo *pinfo);

/* check functions */
int check_section_selected(t_pinfo *pinfo, uint8_t n_sect, uint8_t n_type);
int check_archs_in_file(void *mfile, t_pinfo *pinfo);
int check_arch_in_file(cpu_type_t cputype, cpu_subtype_t cpusubtype, t_pinfo *pinfo, char *arch_name);

int  check_load_command(struct load_command *load_command, t_pinfo *pinfo, uint32_t load_cmd_id);
int  check_lc_symtab(struct load_command *load_command, t_pinfo *pinfo, uint32_t load_cmd_id);
int  check_lc_segment(struct load_command *load_command, t_pinfo *pinfo, uint32_t load_cmd_id);
int  check_lc_segment_64(struct load_command *load_command, t_pinfo *pinfo, uint32_t load_cmd_id);
int  check_macho_file(void *mfile, t_pinfo *pinfo);
int  check_archive_file(void *mfile, t_pinfo *pinfo);
int check_fat_file(void *mfile, t_pinfo *pinfo);

/*  command handle functions  */
int handle_load_command(struct load_command *load_command, t_pinfo *pinfo, void *filestart);

void handle_lc_segment_64(void *addr, t_pinfo *pinfo);
void handle_lc_segment(void *addr, t_pinfo *pinfo);
void handle_lc_symtab(void *addr, t_pinfo *pinfo, void *filestart);

/*  utils functions */

void update_symtab_size(t_pinfo *pinfo, uint32_t addsize);
void update_sectab_size(t_pinfo *pinfo, uint32_t addsize);

/*  sort functions (maybe up section) */

void sort_symtab(t_pinfo *pinfo);

/*        */
int get_sect_names(char segname[16], char sectname[16], t_pinfo *pinfo, uint8_t n_sect);
char  get_symbol(uint8_t n_type, uint8_t n_sect, uint64_t n_value, t_pinfo *pinfo);
void  assign_symbol(t_pinfo *pinfo);

/*        */

uint32_t  get_number_load_command(void *mfile, t_pinfo *pinfo);

/*  error functions */

int     ft_memalloc_error(int ret, size_t size, char *prog_name);
void    ft_throw_error(char *message, ...);
int     ft_return_error(int ret, char *message, ...);
int     ft_usage_error(int ret, char *usage, char *prog_name);
// int     return_memalloc_error(int retu, size_t size, char *prog_name);


#endif
