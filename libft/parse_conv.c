/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:25:03 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:25:03 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*get_flags(char *str, int l)
{
	int		j;
	char	*flags;
	int		f_index;
	int		check;

	if (!(flags = ft_strnew(6)))
		return (NULL);
	f_index = 0;
	while (str[l] && !(ft_isdigit(str[l])))
		l++;
	if (str[l] == '0')
		flags[f_index++] = str[l];
	l = -1;
	while (str[++l])
		if (ft_strchr(FLAGS, str[l]) != NULL && str[l] != '0')
		{
			j = 0;
			check = 0;
			while (j < 6 && flags[j])
				if (flags[j++] == str[l])
					check = 1;
			if (check == 0)
				flags[f_index++] = str[l];
		}
	return (flags);
}

int		get_prec(char *str, int i, va_list ap)
{
	int		j;

	while (i > -1 && str[i] != '.')
		i--;
	if (i == -1)
		return (-1);
	j = i;
	if (str[j + 1] == '*')
	{
		j = va_arg(ap, int);
		if (j < 0)
			return (-1);
		return (j);
	}
	return (ft_atoi(&str[i + 1]));
}

int		get_width(char *str, int j, va_list ap, char **flags)
{
	int		check;
	int		n;

	check = 0;
	while (str[j] && ft_strchr(FLAGS, str[j]))
		j++;
	n = j;
	while (ft_isdigit(str[n]))
		n++;
	if (str[n] == '*')
	{
		j = n;
		n = va_arg(ap, int);
		while (str[j] && ft_strchr("#0-+ hljz*L", str[j]) != NULL)
		{
			j++;
			if (str[j] != '0' && ft_isdigit(str[j]))
				return (ft_atoi(&str[j]));
		}
		if (n < 0)
			make_c_join("-", flags, 0);
		return (n < 0 ? -n : n);
	}
	return (ft_atoi(&str[j]));
}

char	*get_mod(char *str)
{
	char	*tmp;

	if ((tmp = ft_strchr(str, 'j')) != NULL)
		return (ft_strdup("j"));
	if ((tmp = ft_strchr(str, 'z')) != NULL)
		return (ft_strdup("z"));
	if (ft_strchr(str, 'L'))
		return (ft_strdup("ll"));
	if ((tmp = ft_strchr(str, 'l')) != NULL)
	{
		if (*(tmp + 1) && *(tmp + 1) == 'l')
			return (ft_strdup("ll"));
		else
			return (ft_strdup("l"));
	}
	if ((tmp = ft_strchr(str, 'h')) != NULL)
	{
		if (*(tmp + 1) && *(tmp + 1) == 'h')
			return (ft_strdup("hh"));
		else
			return (ft_strdup("h"));
	}
	return (NULL);
}

char	get_type(char type, t_conv *data)
{
	if (type && ft_strchr(TYPES, type))
		return (type);
	data->no_type = type;
	return ('0');
}
