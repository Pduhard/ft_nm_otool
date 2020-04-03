/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_flags.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:25:59 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:25:59 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		display_bsz(t_conv *data, unsigned int c, char **converted)
{
	size_t	len;
	char	*tmp;

	len = ft_strlen(*converted);
	if (c == 0)
	{
		if (len == 0)
			return (1);
		if (data->width > 1)
		{
			if (ft_strchr(data->flags, '-') != NULL)
			{
				*converted[0] = '\0';
				return (len);
			}
			tmp = ft_strdup((*converted) + 1);
			ft_strdel(converted);
			*converted = tmp;
			return (len);
		}
	}
	return (len);
}

void	display_str_prec(char **converted, int prec)
{
	char	*tmp;

	tmp = NULL;
	if (prec < (int)ft_strlen(*converted) && prec > -1)
	{
		tmp = ft_strsub(*converted, 0, prec);
		ft_strdel(converted);
		*converted = tmp;
	}
}

void	display_str_width(char **converted, t_conv *data)
{
	int		len;
	char	*tmp;

	tmp = NULL;
	if (data->width > (len = (int)ft_strlen(*converted)))
	{
		if (ft_strchr(data->flags, '0') != NULL)
			tmp = ft_strfillnew('0', data->width - len);
		else
			tmp = ft_strfillnew(' ', data->width - len);
		if (ft_strchr(data->flags, '-'))
			*converted = ft_strfjoin(*converted, tmp);
		else
			*converted = ft_strfjoin(tmp, *converted);
	}
}

void	display_str_flags(char **converted, t_conv *data)
{
	if (data->type == 's' && !data->mod)
		display_str_prec(converted, data->prec);
	display_str_width(converted, data);
}
