/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fdprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:09:58 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:09:59 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_fdprintf(int fd, const char *format, ...)
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
	ret = print_chain(tmp, fd);
	return (ret);
}
