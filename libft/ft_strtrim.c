/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:21:19 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:21:20 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s)
{
	size_t				end;
	unsigned int		start;
	char				*str;

	start = -1;
	if (!s || !s[0])
		return (ft_strdup(s));
	end = ft_strlen(s) - 1;
	while (s[++start] == ' ' || s[start] == '\n' || s[start] == '\t')
		if (start == end)
		{
			if (!(str = (char*)malloc(sizeof(*str))))
				return (NULL);
			str[0] = '\0';
			return (str);
		}
	while (s[end] == ' ' || s[end] == '\n' || s[end] == '\t')
		end--;
	end = end - start + 1;
	if (!(str = ft_strsub(s, start, end)))
		return (NULL);
	str[end] = '\0';
	return (str);
}
