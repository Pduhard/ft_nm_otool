/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:16:14 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:16:15 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_sqrt(int n, int return_max)
{
	int	min;
	int	max;
	int	test;

	min = 0;
	max = 46341;
	test = (max - min) / 2 + min;
	while (test * test != n)
	{
		if (test * test > n)
			max = test;
		if (test * test < n)
			min = test;
		if (max - min <= 1)
			return (return_max ? max : 0);
		test = (max - min) / 2 + min;
	}
	return (test);
}
