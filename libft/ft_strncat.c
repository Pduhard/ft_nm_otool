/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:20:01 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:20:02 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *s1, const char *s2, int nb)
{
	int		x;
	int		cpt;

	x = 0;
	cpt = 0;
	while (s1[x] != '\0')
		x++;
	while (s2[cpt] != '\0' && cpt < nb)
		s1[x++] = s2[cpt++];
	s1[x] = '\0';
	return (s1);
}
