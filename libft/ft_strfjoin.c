/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strfjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:17:04 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:17:04 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strfjoin(char *s1, char *s2)
{
	char *str;

	if (!(str = ft_strjoin(s1, s2)))
		return (ft_strnew(0));
	ft_strdel(&s1);
	ft_strdel(&s2);
	return (str);
}
