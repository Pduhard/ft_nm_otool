/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:16:50 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:16:51 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strdup(const char *src)
{
	int		index;
	char	*res;

	if (src == NULL)
		return (NULL);
	index = 0;
	while (src[index])
		index++;
	if (!(res = malloc(index + 1)))
		return (NULL);
	index = 0;
	while (*src != '\0')
		res[index++] = *(src++);
	res[index] = '\0';
	return (res);
}
