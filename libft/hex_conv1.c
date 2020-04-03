/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hex_conv1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:23:41 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:23:42 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ull_x_conv(unsigned long long n, int *neg, t_conv *data)
{
	char	*converted;
	char	*tmp;

	if ((tmp = ft_strchr(data->flags, '+')) != NULL)
		*tmp = '9';
	if ((tmp = ft_strchr(data->flags, ' ')) != NULL)
		*tmp = '9';
	if (data->type == 'X')
		converted = ft_baseconvert("0123456789ABCDEF", n);
	else
		converted = ft_baseconvert("0123456789abcdef", n);
	*neg = n == 0 ? 2 : 0;
	return (converted);
}
