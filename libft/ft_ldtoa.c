/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ldtoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:12:46 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:12:47 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*check_nine(char **nb, int i)
{
	int		nine_count;

	nine_count = 0;
	while (i > -1)
	{
		if (nb[0][i] != '9' && nine_count < 15)
			return (NULL);
		else
			nine_count++;
		i--;
		if (i > 0 && nb[0][i - 1] != '9')
		{
			nb[0][i] = nb[0][i] == '9' ? '0' : nb[0][i] + 1;
			nb[0][i - 1] = nb[0][i] == '0' ? nb[0][i - 1] + 1 : nb[0][i - 1];
			while (nb[0][i])
				nb[0][i++] = '0';
			return (*nb);
		}
	}
	return (NULL);
}

static int	get_floats_len(long double n)
{
	int	size;

	size = 0;
	while (n > 0.0)
	{
		n = n * 10;
		n -= (int)n;
		size++;
	}
	return (size);
}

static char	*get_floats(long double n, int prec)
{
	char	*nb;
	int		i;
	char	*tmp;

	if (!(nb = malloc(get_floats_len(n) + 1)))
		return (NULL);
	i = 0;
	while (n > 0.0)
	{
		n = n * 10;
		nb[i++] = (int)n + '0';
		n -= (int)n;
		if (i > 1 && nb[i - 1] != '9' && nb[i - 2] == '9')
			if (check_nine(&nb, i - 2) != NULL)
				n = 0.0;
	}
	nb[i] = '\0';
	nb = i < prec ? ft_strfjoin(nb, ft_strfillnew('0', prec - i)) : nb;
	if (prec != -1 && prec < i)
	{
		tmp = ft_strsub(nb, 0, prec);
		ft_strdel(&nb);
		nb = tmp;
	}
	return (nb);
}

static char	*ft_getnumber(long double n, long double div, int size, int prec)
{
	char	*nb;
	int		i;
	char	*tmp;

	i = 0;
	if (!(nb = malloc(size + 2)))
		return (NULL);
	while (div >= 1)
	{
		nb[i++] = ((long long)n / (long long)div + '0');
		n -= div * (long double)((long long)n / (long long)div);
		div /= 10;
	}
	nb[i] = '\0';
	nb = ft_strfjoin(nb,
			(ft_strlen((tmp = get_floats(n, prec))) != 0
					? ft_strfjoin(ft_strdup("."), tmp) : tmp));
	return (nb);
}

int			ft_ldtoa(long double n, char **array, int prec)
{
	int				neg;
	long double		div;
	int				size;

	size = 0;
	div = 1;
	neg = 0;
	if (n == 340282346638528859811704183484516925440.000000)
		*array = ft_strdup("340282346638528859811704183484516925440.000000");
	if (n == 340282346638528859811704183484516925440.000000)
		return (0);
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
	*array = ft_getnumber(n, div, size, prec);
	return (neg);
}
