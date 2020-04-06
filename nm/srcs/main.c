#include "ft_nm.h"

void *mapp_file(char *file_name, char *bin_name, off_t *fsize)
{
  int         fd;
  struct stat stat_buf;
  void        *mapped_file;

  if ((fd = open(file_name, O_RDONLY)) < 0 || fstat(fd, &stat_buf) < 0)
  {
    ft_fdprintf(2, "error: %s: can't open file: %s\n", bin_name, file_name);
    return (NULL);
  }
  *fsize = stat_buf.st_size;
  if ((mapped_file = mmap(NULL, stat_buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
  {
    ft_fdprintf(2, "error: %s: can't map file: %s\n", bin_name, file_name);
    return (NULL);
  }
  close(fd);
  return (mapped_file);
}

int main(int argc, char **argv)
{
//  int n = 1;
// little endian if true
// if (*(char *)&n == 1)
// {
//   printf("little endian %d\n", n >> 24);
// }
  off_t fsize;
  int     i = 1;
  void    *mfile;

  if (argc < 2)
    return (0); // !!!!!! need to run with a.out !!!!!!
  while (argv[i])
  {
    if ((mfile = mapp_file(argv[i], argv[0], &fsize)))
      display_file_sym(mfile, 0, argv[i], fsize);
    i++;
  }
  return (0);
}
