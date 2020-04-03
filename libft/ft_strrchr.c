/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:20:51 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:20:52 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	chr;
	int		i;

	i = 0;
	while (s[i] != '\0')
		i++;
	chr = (char)c;
	while (s[i] != chr && i > 0)
		i--;
	if (s[i] != chr)
		return (NULL);
	return ((char*)&s[i]);
}
