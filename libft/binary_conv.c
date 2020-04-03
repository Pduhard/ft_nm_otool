/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_conv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:07:18 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:07:20 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		display_binary_prec(char **str, t_conv *data)
{
	int		len;
	char	*tmp;
	int		i;
	int		j;

	j = 0;
	i = 0;
	if (data->prec == 4 || data->prec == 8)
	{
		len = ft_strlen(*str);
		len += len / data->prec - 1;
		if (!(tmp = (char *)malloc(len + 1)))
			return (0);
		while (str[0][i])
		{
			tmp[j++] = str[0][i++];
			if (i % data->prec == 0)
				tmp[j++] = ' ';
		}
		tmp[j] = '\0';
		ft_strdel(str);
		*str = tmp;
	}
	return (0);
}

void	make_binary_conv(char **str, t_conv *data, va_list ap)
{
	if (data->type == 'b' && data->mod)
	{
		if (ft_strcmp(data->mod, "hh") == 0)
			*str = b_conv((unsigned char)va_arg(ap, unsigned int), data, 8);
		else if (ft_strcmp(data->mod, "h") == 0)
			*str = b_conv((unsigned short)va_arg(ap, unsigned int), data, 16);
		else if (ft_strcmp(data->mod, "l") == 0)
			*str = b_conv(va_arg(ap, unsigned long), data, 64);
		else if (ft_strcmp(data->mod, "ll") == 0)
			*str = b_conv(va_arg(ap, unsigned long long), data, 64);
		else if (ft_strcmp(data->mod, "j") == 0)
			*str = b_conv(va_arg(ap, uintmax_t), data, 64);
		else if (ft_strcmp(data->mod, "z") == 0)
			*str = b_conv(va_arg(ap, size_t), data, 64);
	}
	else if (data->type == 'B')
		*str = b_conv(va_arg(ap, unsigned long long), data, 64);
	else
		*str = b_conv(va_arg(ap, unsigned int), data, 32);
	display_binary_prec(str, data);
	display_str_flags(str, data);
	data->size = ft_strlen(*str);
}
