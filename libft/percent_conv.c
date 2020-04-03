/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   percent_conv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:25:26 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:25:27 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	make_percent_conv(char **converted, t_conv *data)
{
	*converted = ft_strdup("%");
	display_str_flags(converted, data);
	data->size = ft_strlen(*converted);
}
