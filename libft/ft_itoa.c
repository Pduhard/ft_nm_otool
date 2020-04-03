/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:12:37 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:12:38 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_getnumber(int n, int div, int neg, int size)
{
	char	*nb;
	int		i;

	i = 0;
	if (neg == -1)
		size++;
	if (!(nb = malloc(size + 2)))
		return (NULL);
	if (neg == -1)
		nb[i++] = '-';
	while (div > 0)
	{
		nb[i++] = n / div + '0';
		n %= div;
		div /= 10;
	}
	nb[i] = '\0';
	return (nb);
}

static char	*ft_returnintmin(void)
{
	char	*min;

	if (!(min = malloc(12)))
		return (NULL);
	min = ft_strcpy(min, "-2147483648");
	return (min);
}

char		*ft_itoa(int n)
{
	int		neg;
	int		div;
	int		size;

	size = 0;
	div = 1;
	neg = 1;
	if (n == -2147483648)
		return (ft_returnintmin());
	if (n < 0)
	{
		neg = -1;
		n *= -1;
	}
	while (n / div > 9)
	{
		size++;
		div *= 10;
	}
	return (ft_getnumber(n, div, neg, size));
}
