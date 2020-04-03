/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_fore_conv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:23:15 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:23:15 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		check_f_conv(char *conv, t_conv *data)
{
	int		point;
	int		i;

	i = 0;
	if ((point = ft_strichr(conv, '.')) > data->prec)
		return (0);
	else if (point == -1 && (int)ft_strlen(conv) > data->prec)
		return (0);
	else if (point == 1)
	{
		if (conv[0] != '0')
			return (1);
		while (conv[i] < '1' || conv[i] > '9')
			i++;
		if (i > 5)
			return (0);
		data->prec += i - 2;
	}
	if (conv[0] != '0' && point != -1)
		data->prec--;
	return (1);
}

int		remove_useless_zero(char **conv, int action)
{
	int		i;
	int		j;
	char	*tmp;

	i = (int)ft_strlen(*conv) - 1;
	j = 0;
	while (conv[0][i] == '0')
	{
		i--;
		j++;
	}
	if (conv[0][i] == '.')
		i--;
	if (action == 1)
	{
		tmp = ft_strsub(*conv, 0, i + 1);
		ft_strdel(conv);
		*conv = tmp;
	}
	if (action == 2)
		ft_strdel(conv);
	return (j);
}

void	spec_sci_conv(char **conv, t_conv *data)
{
	int		i;
	char	*tmp;

	i = 0;
	while (conv[0][i] && (conv[0][i] < '1' || conv[0][i] > '9'))
		i++;
	tmp = ft_strsub(*conv, 0, i + data->prec);
	i = remove_useless_zero(&tmp, 2);
	if (ft_strchr(data->flags, '#'))
		i = 0;
	data->prec -= i + 1;
	display_sci(conv, data);
}

void	make_fore_conv(char **conv, t_conv *data, va_list ap)
{
	int		neg;

	if (data->mod && ft_strcmp(data->mod, "ll") == 0)
		neg = ft_ldtoa(va_arg(ap, long double), conv, -1);
	else
		neg = ft_ldtoa(va_arg(ap, double), conv, -1);
	data->prec = data->prec == -1 ? 6 : data->prec;
	data->prec = data->prec == 0 ? 1 : data->prec;
	if (check_f_conv(*conv, data) == 1)
	{
		ft_strfsub(conv, 0, data->prec + 3);
		round_float(conv, data->prec);
		if (ft_strichr(*conv, '.') == 1 && conv[0][0] != '0')
			round_float(conv, data->prec - 1);
		if (ft_strichr(*conv, '.') != -1 && !ft_strchr(data->flags, '#'))
			remove_useless_zero(conv, 1);
	}
	else
		spec_sci_conv(conv, data);
	display_signed_flags(conv, data, neg);
	data->size = ft_strlen(*conv);
}
