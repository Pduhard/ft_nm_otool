/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp_case.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.le-101.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 07:09:40 by aplat             #+#    #+#             */
/*   Updated: 2020/03/04 07:09:54 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strncmp_case(const char *s1, const char *s2, size_t n)
{
	size_t	cpt;

	cpt = 1;
	if (n < 1)
		return (0);
	cpt = 1;
	while (ft_tolower(*s1) == ft_tolower(*s2) && cpt < n && *s1 != '\0')
	{
		cpt++;
		s1++;
		s2++;
	}
	return ((unsigned char)ft_tolower(*s1) - ft_tolower(*s2));
}
