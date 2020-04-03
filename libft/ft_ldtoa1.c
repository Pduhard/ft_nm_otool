/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ldtoa1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:12:53 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:12:54 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	make_plus_one(char **nb, char **zeros)
{
	char	*tmp;

	tmp = *nb;
	*nb = ft_itoa(ft_atoi(tmp) + 1);
	ft_strdel(&tmp);
	ft_strdel(zeros);
	return (0);
}

static char	*first_float_zero(char *nb)
{
	char	*zeros;
	int		i;
	int		j;

	i = 0;
	while (nb[i] && nb[i] != '.')
		i++;
	j = ++i;
	while (nb[j] == '0')
		j++;
	if (!(zeros = (char *)malloc(j - i + 1)))
		return (NULL);
	j = 0;
	while (nb[i] == '0')
		zeros[j++] = nb[i++];
	zeros[j] = '\0';
	return (zeros);
}

static int	remake_nb(char **nb, char *tmp, char *zeros)
{
	int		floats;
	int		len;
	int		len2;
	char	*tmp2;

	len = ft_strlen(tmp + 1) - 1;
	floats = ft_atoi(tmp + 1) / 10 + 1;
	if (floats == 1 && ft_strlen(zeros) == 0)
		return (make_plus_one(nb, &zeros));
	tmp2 = ft_itoa(floats);
	len2 = ft_strlen(tmp2) + ft_strlen(zeros);
	ft_strdel(&tmp2);
	if (len != -1 && len2 != len)
	{
		tmp = ft_strfjoin(zeros, ft_itoa(floats));
		tmp2 = ft_strfjoin(ft_itoa(ft_atoi(*nb) + 1),
			ft_strfjoin(ft_strdup("."), ft_strdup(tmp + 1)));
		ft_strdel(&tmp);
		ft_strdel(nb);
		*nb = tmp2;
		return (0);
	}
	*(tmp + 1) = '\0';
	*nb = ft_strfjoin(*nb, ft_strfjoin(zeros, ft_itoa(floats)));
	return (0);
}

void		round_float(char **nb, int prec)
{
	int		len;
	char	*tmp;

	len = ft_strlen(*nb);
	if (len - ft_strichr(*nb, '.') - 1 != prec)
	{
		if (nb[0][len - 1] >= '6' && nb[0][len - 1] <= '9')
			remake_nb(nb, ft_strchr(*nb, '.'), first_float_zero(*nb));
		else if (len > 1)
		{
			tmp = ft_strsub(*nb, 0, nb[0][len - 2] == '.' ? len - 2 : len - 1);
			ft_strdel(nb);
			*nb = tmp;
		}
	}
}
