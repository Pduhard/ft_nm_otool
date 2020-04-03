/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_conv1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:08:14 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:08:16 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_intfjoin(unsigned int c, char **converted, int i)
{
	char			*res;
	int				j;

	j = 0;
	res = NULL;
	if (!(res = (char *)malloc(i + 2)))
		return (0);
	res[j++] = (unsigned char)c;
	while (j < i + 1)
	{
		res[j] = converted[0][j - 1];
		j++;
	}
	res[j] = '\0';
	ft_strdel(converted);
	*converted = res;
	return (0);
}

void	get_next_byte(char **converted, unsigned int *c, int len)
{
	unsigned int	tmp;
	int				j;

	j = 0;
	while (j < len)
	{
		tmp = (*c << 26) >> 26;
		if (*c > 31)
			tmp += 128;
		else if (j == 3 && *c < 8)
			tmp += 240;
		else if (j == 2 && *c < 16)
			tmp += 224;
		else if (j == 1 && *c < 32)
			tmp += 192;
		*c = *c >> 6;
		ft_intfjoin(tmp, converted, j);
		j++;
	}
}

int		check_valid_unicode(int len, char *unicode)
{
	if ((unsigned char)unicode[0] == 224 && len == 3)
		if ((unsigned char)unicode[1] < 160)
			return (-1);
	if ((unsigned char)unicode[0] == 237 && len == 3)
		if ((unsigned char)unicode[1] > 159)
			return (-1);
	if ((unsigned char)unicode[0] == 240 && len == 4)
		if ((unsigned char)unicode[1] < 144)
			return (-1);
	if ((unsigned char)unicode[0] == 244 && len == 4)
		if ((unsigned char)unicode[1] > 143)
			return (-1);
	return (len);
}

int		get_unicode_len(unsigned int c)
{
	int len;

	len = -1;
	if (c < 2048)
		len = 2;
	else if (c < 65536)
		len = 3;
	else if (c < 1114112)
		len = 4;
	return (len);
}

char	*wchart_conv(unsigned int c, t_conv *data, int *ln)
{
	char	*converted;
	int		len;

	converted = NULL;
	len = 1;
	if (c < 128 || (c < 256 && MB_CUR_MAX == 1))
		ft_intfjoin(c, &converted, 0);
	else
		len = get_unicode_len(c);
	if (len > MB_CUR_MAX || len == -1)
	{
		ft_strdel(&converted);
		return (NULL);
	}
	if (c > 127 && c < 1114112 && MB_CUR_MAX > 1)
		get_next_byte(&converted, &c, len);
	if (len >= 3)
		len = check_valid_unicode(len, converted);
	if (len == -1)
		ft_strdel(&converted);
	*ln = len;
	data->size = (len == -1 ? len : data->size + len);
	return (converted);
}
