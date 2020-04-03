/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:20:09 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:20:10 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t cpt;

	cpt = 1;
	if (n < 1)
		return (0);
	while (*s1 == *s2 && cpt < n && *s1 != '\0')
	{
		cpt++;
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - *s2);
}
