/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atold.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:09:00 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:09:01 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static double	ft_atold_check_neg(const char **str)
{
	if (**str == '+')
	{
		(*str)++;
		return (1);
	}
	if (**str == '-')
	{
		(*str)++;
		return (-1);
	}
	return (1);
}

long double		ft_atold(const char *str)
{
	long double	itg;
	long double	flt;
	double		neg;
	long double	div;

	div = 10;
	itg = 0;
	flt = 0;
	if (!str)
		return (0);
	while (ft_isspace(*str))
		++str;
	neg = ft_atold_check_neg(&str);
	while (*str && *str != '.' && ft_isdigit(*str))
		itg = itg * 10 + (*str++ - '0');
	if (*str != '.')
		return (itg * neg);
	str++;
	while (*str && ft_isdigit(*str))
	{
		flt += (long double)(*str++ - '0') / div;
		div *= 10;
	}
	return ((flt + itg) * neg);
}
