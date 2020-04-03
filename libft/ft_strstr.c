/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:21:03 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:21:04 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	int cpt;
	int ghost_cpt;

	cpt = 0;
	if (needle[0] == '\0')
		return ((char*)haystack);
	while (haystack[cpt] != '\0')
	{
		if (haystack[cpt] == needle[0])
		{
			ghost_cpt = 0;
			while (needle[ghost_cpt] != '\0')
			{
				if (needle[ghost_cpt] != haystack[cpt + ghost_cpt])
					break ;
				ghost_cpt++;
			}
			if (needle[ghost_cpt] == '\0')
				return ((char*)haystack + cpt);
		}
		cpt++;
	}
	return (NULL);
}
