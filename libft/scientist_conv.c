/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scientist_conv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:25:32 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:25:32 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	supp_ten_conv(char *src, char **sci, int pow, t_conv *data)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	j = 0;
	sci[0][i++] = src[j++];
	if (data->prec != 0)
		sci[0][i++] = '.';
	while (src[j])
	{
		if (src[j] == '.')
			j++;
		if (i < data->prec + 2)
			sci[0][i++] = src[j];
		j++;
	}
	while (data->prec != 0 && i < data->prec + 2)
		sci[0][i++] = '0';
	sci[0][i++] = data->type == 'e' || data->type == 'g' ? 'e' : 'E';
	sci[0][i++] = '+';
	sci[0][i] = '\0';
	*sci = ft_strfjoin(*sci,
			ft_strlen(tmp = ft_itoa(pow)) == 1 ?
			ft_strfjoin(ft_strdup("0"), tmp) : tmp);
}

void	eof_inften_conv(int j, char **sci, char *src, t_conv *data)
{
	char	*tmp;
	int		i;
	int		pow;

	pow = j != 0 ? -j + 1 : 0;
	i = 0;
	sci[0][i++] = src[j];
	sci[0][i++] = '.';
	if (src[j + 1] == '.')
		j++;
	while (src[++j])
		if (i < data->prec + 3)
			sci[0][i++] = src[j];
	while (i < data->prec + 3)
		sci[0][i++] = '0';
	sci[0][i] = '\0';
	round_float(sci, data->prec);
	*sci = ft_strfjoin(ft_strfjoin(*sci,
		ft_strfjoin(ft_strdup(data->type == 'e'
			|| data->type == 'g' ? "e" : "E"),
				ft_strdup(pow == 0 ? "+" : "-"))),
					ft_strlen(tmp = ft_itoa(-pow)) == 1 ?
						ft_strfjoin(ft_strdup("0"), tmp) : tmp);
}

void	inf_ten_conv(char *src, char **sci, t_conv *data)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (src[j] && ft_strchr("123456789", src[j]) == NULL)
		j++;
	if (src[j] == '\0')
		*sci = ft_strfjoin(*sci,
				ft_strfjoin(ft_strfjoin(ft_strdup(data->prec != 0 ?
					"0." : "0"), ft_strfillnew('0', data->prec)),
						ft_strdup(data->type == 'e' || data->type == 'g'
							? "e+00" : "E+00")));
	else
		eof_inften_conv(j, sci, src, data);
}

int		display_sci(char **converted, t_conv *data)
{
	char	*sci;
	int		point;
	int		pow;
	int		i;
	int		j;

	i = 0;
	pow = 0;
	j = 0;
	if (!(sci = (char *)malloc(2 + data->prec + 2 + 1)))
		return (-1);
	point = ft_strichr(*converted, '.');
	sci[0] = '\0';
	if (point == -1 || point != 1)
		supp_ten_conv(*converted, &sci, point == -1 ?
				ft_strlen(*converted) - 1 : point - 1, data);
	else
		inf_ten_conv(*converted, &sci, data);
	ft_strdel(converted);
	*converted = sci;
	return (0);
}

void	make_sci_conv(char **conv, t_conv *data, va_list ap)
{
	int		neg;

	if (data->mod && ft_strcmp(data->mod, "ll") == 0)
		neg = ft_ldtoa(va_arg(ap, long double), conv, -1);
	else
		neg = ft_ldtoa(va_arg(ap, double), conv, -1);
	data->prec = data->prec == -1 ? 6 : data->prec;
	display_sci(conv, data);
	display_signed_flags(conv, data, neg);
	data->size = ft_strlen(*conv);
}
