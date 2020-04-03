/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:19:45 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:19:46 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	char	*str;
	int		i;
	int		len;

	if (s != NULL)
	{
		len = (int)ft_strlen((char*)s);
		if (!(str = malloc(len + 1)))
			return (NULL);
		i = -1;
		while (++i < len)
			str[i] = (*f)(s[i]);
		str[i] = '\0';
		return (str);
	}
	return (NULL);
}
