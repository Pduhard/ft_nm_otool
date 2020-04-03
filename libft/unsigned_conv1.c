/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsigned_conv1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:26:09 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:26:10 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ull_conv(unsigned long long n, t_conv *data)
{
	char	*converted;
	char	*tmp;

	if ((tmp = ft_strchr(data->flags, ' ')) != NULL)
		*tmp = '9';
	if ((tmp = ft_strchr(data->flags, '+')) != NULL)
		*tmp = '9';
	ft_ulltoa(n, &converted);
	return (converted);
}
