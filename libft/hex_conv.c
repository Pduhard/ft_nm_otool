/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hex_conv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:23:26 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:23:26 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*display_hex_tag(char pre, int size, t_conv *data, char **converted)
{
	char	*res;
	char	*tst;

	tst = NULL;
	if ((tst = ft_strchr(data->flags, '#')))
		size -= 2;
	res = ft_strfillnew(pre, size);
	if (tst && ft_strlen(*converted))
	{
		if (pre == ' ')
		{
			if (ft_strchr(data->flags, '-') == NULL)
				res = ft_strfjoin(res, (data->type == 'X' ?
							ft_strdup("0X") : ft_strdup("0x")));
		}
		else
			make_c_join((data->type == 'X' ? "0X" : "0x"), &res, 0);
		data->type = 42;
	}
	return (res);
}

int		display_hex_width(int zero, int *neg, t_conv *data, char **converted)
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;
	char	*tmp4;
	int		l;

	if (data->width > (l = (int)ft_strlen(*converted)) + *neg)
	{
		if ((tmp4 = ft_strchr(data->flags, '+')) != NULL)
			l++;
		if ((tmp3 = ft_strchr(data->flags, '0')) != NULL
				&& ft_strchr(data->flags, '-') == NULL
				&& zero == 1 && tmp4 == NULL && data->prec == -1)
			tmp = display_hex_tag('0', data->width - l - *neg, data, converted);
		else
			tmp = display_hex_tag(' ', data->width - l - *neg, data, converted);
		*neg = (tmp4 != NULL ? make_c_join("+", converted, 2) : *neg);
		tmp2 = *converted;
		*converted = (ft_strchr(data->flags, '-') != NULL) ?
			ft_strfjoin(tmp2, tmp) : ft_strfjoin(tmp, tmp2);
		return (ft_strchr(data->flags, '-') == NULL ? 1 : 0);
	}
	return (0);
}

void	display_hex_flags(char **converted, t_conv *data, int *neg)
{
	int		len;
	char	*tmp;
	char	*tmp2;
	int		modified;
	int		zero;

	zero = *neg == 2 ? check_zero(converted, data) : 0;
	*neg = 0;
	tmp = NULL;
	display_prec(converted, data->prec, &len);
	if (data->prec < len)
		zero = 1;
	else if (data->prec != -1)
		zero = 2;
	modified = display_hex_width(zero, neg, data, converted);
	if (modified != 1 && ft_strchr(data->flags, '#') != NULL)
		make_c_join(data->type == 'X' ? "0X" : "0x", converted, 0);
	if (ft_strchr(data->flags, '+') != NULL)
		tmp = ft_strfillnew('+', 1);
	else
		display_space(converted, data->flags, modified);
	tmp2 = *converted;
	if (tmp != NULL)
		*converted = ft_strfjoin(tmp, tmp2);
}

void	make_hex_conv(char **str, t_conv *dt, va_list ap)
{
	int	z;

	z = 0;
	*str = NULL;
	if (dt->mod)
	{
		if (ft_strcmp(dt->mod, "hh") == 0)
			*str = ull_x_conv((unsigned char)va_arg(ap, unsigned int), &z, dt);
		else if (ft_strcmp(dt->mod, "h") == 0)
			*str = ull_x_conv((unsigned short)va_arg(ap, unsigned int), &z, dt);
		else if (ft_strcmp(dt->mod, "l") == 0)
			*str = ull_x_conv(va_arg(ap, unsigned long), &z, dt);
		else if (ft_strcmp(dt->mod, "ll") == 0)
			*str = ull_x_conv(va_arg(ap, unsigned long long), &z, dt);
		else if (ft_strcmp(dt->mod, "j") == 0)
			*str = ull_x_conv(va_arg(ap, uintmax_t), &z, dt);
		else if (ft_strcmp(dt->mod, "z") == 0)
			*str = ull_x_conv(va_arg(ap, size_t), &z, dt);
	}
	else
		*str = ull_x_conv(va_arg(ap, unsigned int), &z, dt);
	display_hex_flags(str, dt, &z);
	dt->size = ft_strlen(*str);
}

void	make_ptr_conv(char **converted, t_conv *data, va_list ap)
{
	int		neg;
	char	*tmp;

	neg = 2;
	*converted = NULL;
	make_c_join("#", &data->flags, 0);
	if ((tmp = ft_strchr(data->flags, '+')) != NULL)
		*tmp = '9';
	if ((tmp = ft_strchr(data->flags, ' ')) != NULL)
		*tmp = '9';
	data->type = 'p';
	*converted = ull_x_conv(va_arg(ap, unsigned long), &neg, data);
	display_hex_flags(converted, data, &neg);
	data->size = ft_strlen(*converted);
}
