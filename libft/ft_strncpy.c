/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:20:30 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:20:30 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	size_t cpt;

	cpt = 0;
	while (src[cpt] != '\0' && cpt < len)
	{
		dst[cpt] = src[cpt];
		cpt++;
	}
	while (cpt < len)
		dst[cpt++] = '\0';
	return (dst);
}
