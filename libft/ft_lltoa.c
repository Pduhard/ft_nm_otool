/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lltoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:11:44 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:11:44 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_getnumber(long long n, long long div, int size, char **array)
{
	char	*nb;
	int		i;

	i = 0;
	if (!(nb = malloc(size + 2)))
		array = NULL;
	while (div > 0)
	{
		nb[i++] = n / div + '0';
		n %= div;
		div /= 10;
	}
	nb[i] = '\0';
	*array = nb;
}

static int	ft_returnintmin(char **array)
{
	char	*min;

	if (!(min = malloc(20)))
		return (0);
	*array = ft_strcpy(min, "9223372036854775808");
	return (1);
}

int			ft_lltoa(long long n, char **array)
{
	int				neg;
	long long		div;
	int				size;

	size = 0;
	div = 1;
	neg = 0;
	if (n == -9223372036854775807 - 1)
		return (ft_returnintmin(array));
	if (n < 0)
	{
		neg = 1;
		n *= -1;
	}
	while (n / div > 9)
	{
		size++;
		div *= 10;
	}
	ft_getnumber(n, div, size, array);
	return (neg);
}
