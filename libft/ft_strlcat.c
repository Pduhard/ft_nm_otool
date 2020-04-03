/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:19:33 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:19:35 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	int	i;
	int	a;
	int	return_length;

	a = 0;
	i = 0;
	while (dst[i] != '\0')
		i++;
	while (src[a] != '\0')
		a++;
	return_length = (int)size < i ? a + (int)size : i + a;
	a = -1;
	while (src[++a] != '\0' && a + i < (int)size - 1)
		dst[i + a] = src[a];
	dst[i + a] = '\0';
	return (return_length);
}
