/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:18:00 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:18:01 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy(char *dest, const char *src)
{
	int cpt;

	cpt = -1;
	while (src[++cpt] != '\0')
		dest[cpt] = src[cpt];
	dest[cpt] = '\0';
	return (dest);
}
