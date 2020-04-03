/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:17:20 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:17:20 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striter(char *s, void (*f)(char*))
{
	size_t len;
	size_t i;

	if (s != NULL && (*f) != NULL)
	{
		i = 0;
		len = ft_strlen(s);
		while (i < len)
			(*f)(&s[i++]);
	}
}
