/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:14:24 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:14:24 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	void	*mem;
	char	*tmp;

	if (!(mem = malloc(size)))
		return (NULL);
	tmp = (char*)mem;
	while (size-- > 0)
		*(tmp++) = '\0';
	return (mem);
}
