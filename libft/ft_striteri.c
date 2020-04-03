/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:17:25 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:17:26 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	size_t len;
	size_t i;

	if (s != NULL && (*f) != NULL)
	{
		len = ft_strlen(s);
		i = -1;
		while (++i < len)
			(*f)((unsigned int)i, &s[i]);
	}
}
