/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:19:50 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:19:51 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		len;
	int		i;
	char	*str;

	if (s != NULL)
	{
		len = (int)ft_strlen((char*)s);
		if (!(str = malloc(len + 1)))
			return (NULL);
		i = -1;
		while (++i < len)
			str[i] = (*f)((unsigned int)i, s[i]);
		str[i] = '\0';
		return (str);
	}
	return (NULL);
}
