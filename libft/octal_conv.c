/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octal_conv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:24:52 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:24:53 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	display_octal_flags(char **converted, t_conv *data, int *neg)
{
	int		len;
	char	*tmp;
	char	*tmp2;
	int		modified;
	int		zero;

	zero = check_zero(converted, data);
	tmp = NULL;
	if (ft_strchr(data->flags, '#') && (data->prec == 0 || ft_atoi(*converted)))
		make_c_join("0", converted, 0);
	modified = display_prec(converted, data->prec, &len);
	if (data->prec < len)
		zero = 1;
	else if (data->prec != -1)
		zero = 2;
	modified = (display_width(zero, neg, data, converted) == 1 ? 1 : modified);
	if (ft_strchr(data->flags, '+') != NULL)
		tmp = ft_strfillnew('+', 1);
	else
		display_space(converted, data->flags, modified);
	tmp2 = *converted;
	if (tmp != NULL)
		*converted = ft_strfjoin(tmp, tmp2);
}

void	make_octal_conv(char **str, t_conv *data, va_list ap)
{
	int	neg;

	neg = 0;
	*str = NULL;
	if (data->type == 'o' && data->mod)
	{
		if (ft_strcmp(data->mod, "hh") == 0)
			*str = ull_o_conv((unsigned char)va_arg(ap, unsigned int), data);
		else if (ft_strcmp(data->mod, "h") == 0)
			*str = ull_o_conv((unsigned short)va_arg(ap, unsigned int), data);
		else if (ft_strcmp(data->mod, "l") == 0)
			*str = ull_o_conv(va_arg(ap, unsigned long), data);
		else if (ft_strcmp(data->mod, "ll") == 0)
			*str = ull_o_conv(va_arg(ap, unsigned long long), data);
		else if (ft_strcmp(data->mod, "j") == 0)
			*str = ull_o_conv(va_arg(ap, uintmax_t), data);
		else if (ft_strcmp(data->mod, "z") == 0)
			*str = ull_o_conv(va_arg(ap, size_t), data);
	}
	else if (data->type == 'O')
		*str = ull_o_conv(va_arg(ap, unsigned long long), data);
	else
		*str = ull_o_conv(va_arg(ap, unsigned int), data);
	display_octal_flags(str, data, &neg);
	data->size = ft_strlen(*str);
}
