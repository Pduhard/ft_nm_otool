/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:10:03 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:10:04 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		bsz_fjoin(char **converted, char *str, int len)
{
	int		bzh_count;
	int		i;
	char	*to_join;

	bzh_count = 0;
	i = 0;
	if (!(to_join = (char *)malloc(len + 1)))
		return (0);
	while (i < len - bzh_count)
	{
		if (str[i + bzh_count] == '\0')
			bzh_count++;
		else
		{
			to_join[i] = str[i + bzh_count];
			i++;
		}
	}
	to_join[i] = '\0';
	*converted = ft_strfjoin(*converted, to_join);
	return (i);
}

int		get_chain(char **converted, t_part *wlst)
{
	int		ret;
	int		retsum;
	t_part	*tmp;
	int		dead;

	ret = 0;
	dead = 0;
	while (wlst)
	{
		retsum = wlst->conv == NULL ?
			(int)ft_strlen((char*)wlst->res) : (int)wlst->conv->size;
		if ((wlst->next != NULL && wlst->next->res == NULL)
				|| wlst->res == NULL)
			dead = 1;
		if (dead != 1)
			ret += bsz_fjoin(converted, wlst->res, retsum);
		tmp = wlst->next;
		free_struct(wlst);
		wlst = tmp;
	}
	return (ret);
}

int		ft_getf(char **converted, const char *format, ...)
{
	t_part	*wlst;
	t_part	*tmp;
	va_list	ap;
	int		ret;
	char	*str;

	va_start(ap, format);
	wlst = NULL;
	str = NULL;
	if (format)
		str = apply_font((char *)format, ap);
	if (format)
		wlst = parse_format(str, ap);
	tmp = wlst;
	ft_strdel(&str);
	while (wlst)
	{
		if (wlst->res == NULL)
			wlst->res = make_conv(wlst->conv, ap);
		wlst = wlst->next;
	}
	va_end(ap);
	ret = get_chain(converted, tmp);
	return (ret);
}
