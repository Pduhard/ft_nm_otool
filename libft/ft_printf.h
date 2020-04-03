/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:14:03 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:14:04 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "libft.h"
# include <stdarg.h>
# include <wchar.h>
# define FLAGS "#0-+ '"
# define MODS "hljzL"
# define TYPES "sSpdDioOuUxXcCfFbBeEgGn%"
# define SIGNED "dDi"
# define UNSIGNED "uU"
# define OCTAL "oO"
# define HEX "xX"
# define CHAR "cC"
# define STR "sS"
# define FLOAT "fF"
# define BIN "bB"
# define SCI "eE"
# define FSCI "gG"
# define VALID "#0-+ 'hljzLsSpdDioOuUxXcCfFbBeEgGn.*"
# define EFFECT "EGSIU"
# define COLORS "NRGYBMCW"

int					ft_printf(const char *format,
						...);

typedef	struct		s_conv
{
	char			*flags;
	int				width;
	int				prec;
	char			*mod;
	char			type;
	size_t			size;
	char			no_type;
	long long		*n_conv;
}					t_conv;

typedef	struct		s_part
{
	struct s_part	*next;
	t_conv			*conv;
	char			*res;
}					t_part;

int					ft_getf(char **converted, const char *format, ...);
int					ft_fdprintf(int fd, const char *format, ...);
t_part				*get_next_part(char *str, int *i, int j, va_list ap);
t_part				*parse_format(char *format, va_list ap);
char				*get_flags(char *str, int l);
int					get_width(char *str, int j, va_list ap, char **flags);
int					get_prec(char *str, int i, va_list ap);
char				*get_mod(char *str);
char				get_type(char type, t_conv *data);
void				make_signed_conv(char **converted,
						t_conv *data, va_list ap);
void				make_unsigned_conv(char **converted,
						t_conv *data, va_list ap);
void				make_octal_conv(char **converted,
						t_conv *data, va_list ap);
void				make_hex_conv(char **converted,
						t_conv *data, va_list ap);
void				make_char_conv(char **converted,
						t_conv *data, va_list ap);
void				make_str_conv(char **converted,
						t_conv *data, va_list ap);
void				make_ptr_conv(char **converted,
						t_conv *data, va_list ap);
void				make_float_conv(char **converted,
						t_conv *data, va_list ap);
void				make_binary_conv(char **converted,
						t_conv *data, va_list ap);
void				make_sci_conv(char **converted,
						t_conv *data, va_list ap);
void				make_fore_conv(char **converted,
						t_conv *data, va_list ap);
void				make_percent_conv(char **converted,
						t_conv *data);
void				make_no_type_conv(char **converted,
						t_conv *data);
void				make_n_conv(char **converted,
						t_conv *data, va_list ap);
void				get_n_value(int sum, t_conv *data);
int					make_c_join(char *c, char **converted, int ret);
char				*long_long_conv(long long n, int *neg, t_conv *data);
char				*ull_conv(unsigned long long n, t_conv *data);
char				*wchart_conv(unsigned int c, t_conv *data, int *ln);
char				*ull_o_conv(unsigned long long n, t_conv *data);
char				*b_conv(unsigned long long n, t_conv *data, int size);
char				*ull_x_conv(unsigned long long n, int *neg, t_conv *data);
void				display_flags(char **converted, t_conv *data, int *neg);
void				display_signed_flags(char **converted,
						t_conv *data, int neg);
void				display_str_flags(char **converted, t_conv *data);
void				display_space(char **converted, char *flags, int modified);
int					display_prec(char **converted, int prec, int *len);
int					display_bsz(t_conv *data, unsigned int c, char **converted);
int					display_width(int zero, int *neg,
						t_conv *data, char **converted);
int					display_hex_width(int zero, int *neg,
						t_conv *data, char **converted);
int					display_sci(char **converted, t_conv *data);
int					display_separator(char **converted);
int					check_zero(char **converted, t_conv *data);
void				free_struct(t_part *part);
char				*make_conv(t_conv *data, va_list ap);
char				*apply_font(char *format, va_list ap);
int					print_chain(t_part *wlst, int fd);

#endif
