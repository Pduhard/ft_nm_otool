/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:08:29 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:08:30 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*color_conv(char *format, int *i, int *check)
{
	int		code;
	int		color;

	code = format[*i] == 'F' ? 30 : 40;
	if (format[*i + 1] && format[*i + 1] == '2')
	{
		(*i)++;
		code += 60;
	}
	if (format[*i + 1] && (color = ft_strichr(COLORS, format[*i + 1])) != -1)
	{
		code += color;
		(*i)++;
	}
	else
		*check = 1;
	return (ft_strfjoin(ft_itoa(code), ft_strdup(";")));
}

char	*rgb_conv(char *format, int *i, va_list ap, int *check)
{
	char	*res;
	int		j;

	j = 0;
	res = NULL;
	if (format[*i + 1] && (format[*i + 1] == 'F' || format[*i + 1] == 'B'))
	{
		res = ft_strdup(format[*i + 1] == 'F' ? "38;2;" : "48;2;");
		(*i)++;
	}
	else
		*check = 1;
	if (*check != 1)
		while (j++ < 3)
		{
			res = ft_strfjoin(res, ft_itoa((unsigned char)va_arg(ap, int)));
			res = ft_strfjoin(res, ft_strdup(";"));
		}
	return (res);
}

char	*effect_conv(char c)
{
	if (c == 'E')
		return (ft_strdup("0;"));
	if (c == 'G')
		return (ft_strdup("1;"));
	if (c == 'S')
		return (ft_strdup("2;"));
	if (c == 'I')
		return (ft_strdup("3;"));
	if (c == 'U')
		return (ft_strdup("4;"));
	return (ft_strdup("WAOU SUPER CE FLAG WERROR QUI SERT A RIEN :)"));
}

char	*is_valid_font(char *format, int *i, va_list ap)
{
	char	*conv;
	int		check;

	check = 0;
	conv = ft_strdup("\x1b[");
	while (format[*i] && format[*i] != '}' && format[*i] != '{')
	{
		if (check == 0 && ft_strchr(EFFECT, format[*i]) != NULL)
			conv = ft_strfjoin(conv, effect_conv(format[*i]));
		else if (check == 0 && (format[*i] == 'F' || format[*i] == 'B'))
			conv = ft_strfjoin(conv, color_conv(format, i, &check));
		else if (check == 0 && format[*i] == 'M')
			conv = ft_strfjoin(conv, rgb_conv(format, i, ap, &check));
		else
			check = 1;
		(*i)++;
	}
	conv = ft_strfjoin(conv, ft_strdup("m"));
	if (check == 1 || format[*i] != '}' || ft_strlen(conv) == 3)
		ft_strdel(&conv);
	if (format[*i] == '{')
		(*i)--;
	return (conv);
}

char	*apply_font(char *format, va_list ap)
{
	int		i;
	char	*str;
	char	*tmp;
	int		start;

	start = 0;
	i = 0;
	str = NULL;
	while (format[i])
	{
		if (format[i] == '{')
		{
			str = ft_strfjoin(str, ft_strsub(format, start, i - start));
			start = i++;
			if ((tmp = is_valid_font(format, &i, ap)) != NULL)
				str = ft_strfjoin(str, tmp);
			else
				str = ft_strfjoin(str, ft_strsub(format, start, i + 1 - start));
			start = i + 1;
		}
		i++;
	}
	str = ft_strfjoin(str, ft_strsub(format, start, i));
	return (str);
}
