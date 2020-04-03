/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_baseconvert.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:09:12 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:09:14 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_baseconvert(char *base, unsigned long long n)
{
	int						size;
	int						bs;
	char					*res;
	unsigned long long		n_cp;

	n_cp = n;
	size = 0;
	bs = (int)ft_strlen(base);
	if (n == 0)
		return (ft_strfillnew('0', 1));
	while (n_cp != 0)
	{
		n_cp /= bs;
		size++;
	}
	if (!(res = malloc(size + 1)))
		return (NULL);
	res[size] = '\0';
	while (size-- > 0)
	{
		res[size] = base[n % bs];
		n /= bs;
	}
	return (res);
}
