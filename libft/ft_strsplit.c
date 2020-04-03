/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:20:58 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:20:58 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*get_next_word(char const *s, int *j, char c)
{
	int		len;
	int		k;
	char	*word;

	while (s[*j] == c)
		*j += 1;
	k = *j;
	len = 0;
	while (s[k] != c && s[k] != '\0')
		k++;
	if (!(word = (char*)malloc(sizeof(char) * (k - *j + 1))))
		return (NULL);
	k = 0;
	while (s[*j] != c && s[*j] != '\0')
	{
		word[k++] = s[*j];
		*j += 1;
	}
	word[k] = '\0';
	return (word);
}

char		**ft_strsplit(char const *s, char c)
{
	int		wrd;
	char	**tab;
	int		i;
	int		j;

	if (s == NULL)
		return (NULL);
	i = 0;
	j = 0;
	wrd = ft_countparts(c, s);
	if (!(tab = (char**)malloc(sizeof(char*) * (wrd + 1))))
		return (NULL);
	while (i < wrd)
		tab[i++] = get_next_word(s, &j, c);
	tab[i] = 0;
	return (tab);
}
