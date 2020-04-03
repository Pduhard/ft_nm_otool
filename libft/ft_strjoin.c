/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:19:25 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:19:26 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, char const *s2)
{
	size_t	len;
	char	*str;
	int		i;
	int		j;

	j = 0;
	i = 0;
	len = ft_strlen(s1) + ft_strlen(s2);
	if (!(str = malloc(len + 1)))
		return (NULL);
	while (s1 && s1[i] != '\0')
		str[j++] = s1[i++];
	i = 0;
	while (s2 && s2[i] != '\0')
		str[j++] = s2[i++];
	str[j] = '\0';
	return (str);
}
