/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strfillnew.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:17:00 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:17:00 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strfillnew(char c, int len)
{
	char	*str;
	int		i;

	i = 0;
	if (len <= 0)
		return (ft_strnew(0));
	if (!(str = (char *)malloc(len + 1)))
		return (NULL);
	while (i < len)
		str[i++] = c;
	str[i] = '\0';
	return (str);
}
