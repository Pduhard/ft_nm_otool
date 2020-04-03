/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strichr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:17:14 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:17:14 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strichr(const char *s, int c)
{
	char	chr;
	int		i;

	i = 0;
	chr = (char)c;
	if (s == NULL)
		return (0);
	while (*s != chr && *s != '\0')
	{
		s++;
		i++;
	}
	if (*s != chr)
		return (-1);
	return (i);
}
