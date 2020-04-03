/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strclr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:17:47 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:17:47 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strclr(char *s)
{
	size_t len;
	size_t i;

	if (s != NULL)
	{
		i = 0;
		len = ft_strlen(s);
		while (i < len)
			s[i++] = '\0';
	}
}