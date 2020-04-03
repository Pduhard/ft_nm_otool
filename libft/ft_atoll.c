/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:09:06 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:09:07 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long	ft_atoll(const char *str)
{
	unsigned long long	nb;
	int					neg;
	int					check;

	neg = 1;
	nb = 0;
	check = 0;
	while (str && ((*str >= 9 && *str <= 13) || *str == 32) && (check = 1))
		str++;
	if (str && *str == '+' && (check = 1))
		str++;
	if (str && *str == '-' && (check != 1 || *(str - 1) != '+') && (neg = -1))
		str++;
	while (str && *str >= '0' && *str <= '9')
	{
		nb = nb * 10 + *str - 48;
		str++;
	}
	if (nb > 9223372036854775807 && neg == 1)
		return (-1);
	if (nb - 1 > 9223372036854775807 && neg == -1)
		return (0);
	return ((long long)(nb * neg));
}
