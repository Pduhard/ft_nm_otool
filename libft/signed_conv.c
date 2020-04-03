/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signed_conv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:25:36 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:25:37 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	display_flags(char **converted, t_conv *data, int *neg)
{
	int		len;
	char	*tmp;
	char	*tmp2;
	int		modified;
	int		zero;

	zero = 0;
	if (ft_strchr("idDuUxXoO", data->type) != NULL)
		zero = check_zero(converted, data);
	tmp = NULL;
	modified = display_prec(converted, data->prec, &len);
	if (data->prec < len)
		zero = 1;
	else if (data->prec != -1)
		zero = 2;
	modified = (display_width(zero, neg, data, converted) == 1 ? 1 : modified);
	if (ft_strchr(data->flags, '+') != NULL && *neg == 0)
		tmp = ft_strfillnew('+', 1);
	else if (*neg == 1)
		tmp = ft_strfillnew('-', 1);
	else
		display_space(converted, data->flags, modified);
	tmp2 = *converted;
	if (tmp != NULL)
		*converted = ft_strfjoin(tmp, tmp2);
}

void	make_signed_conv(char **converted, t_conv *data, va_list ap)
{
	int	neg;

	neg = 0;
	*converted = NULL;
	if ((data->type == 'd' || data->type == 'i') && data->mod)
	{
		if (ft_strcmp(data->mod, "l") == 0 || ft_strcmp(data->mod, "z") == 0)
			*converted = long_long_conv(va_arg(ap, long), &neg, data);
		else if (ft_strcmp(data->mod, "hh") == 0)
			*converted = long_long_conv((char)va_arg(ap, int), &neg, data);
		else if (ft_strcmp(data->mod, "h") == 0)
			*converted = long_long_conv((short)va_arg(ap, int), &neg, data);
		else if (ft_strcmp(data->mod, "ll") == 0)
			*converted = long_long_conv(va_arg(ap, long long), &neg, data);
		else if (ft_strcmp(data->mod, "j") == 0)
			*converted = long_long_conv(va_arg(ap, intmax_t), &neg, data);
	}
	else if (data->type == 'D')
		*converted = long_long_conv(va_arg(ap, long long), &neg, data);
	else
		*converted = long_long_conv(va_arg(ap, int), &neg, data);
	display_flags(converted, data, &neg);
	data->size = ft_strlen(*converted);
}
