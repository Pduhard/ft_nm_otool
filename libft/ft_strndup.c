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

#include "libft.h"

char	*ft_strndup(const char *src, size_t len)
{
	size_t	index;
	char	  *res;

	if (src == NULL)
		return (NULL);
	index = 0;
	while (src[index] && index < len)
		index++;
	if (!(res = malloc(index + 1)))
		return (NULL);
	index = 0;
	while (src[index] && index < len)
  {
		res[index] = src[index];
    index++;
  }
  res[index] = '\0';
	return (res);
}
