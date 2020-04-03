/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:13:51 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:13:56 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*make_conv2(t_conv *data, va_list ap)
{
	char	*res;

	res = NULL;
	if (ft_strchr(FLOAT, data->type) != NULL)
		make_float_conv(&res, data, ap);
	else if (ft_strchr(BIN, data->type) != NULL)
		make_binary_conv(&res, data, ap);
	else if (ft_strchr(SCI, data->type) != NULL)
		make_sci_conv(&res, data, ap);
	else if (ft_strchr(FSCI, data->type) != NULL)
		make_fore_conv(&res, data, ap);
	else if (data->type == 'n')
		make_n_conv(&res, data, ap);
	return (res);
}

char	*make_conv(t_conv *data, va_list ap)
{
	char	*res;

	res = NULL;
	if (ft_strchr(SIGNED, data->type) != NULL)
		make_signed_conv(&res, data, ap);
	else if (ft_strchr(UNSIGNED, data->type) != NULL)
		make_unsigned_conv(&res, data, ap);
	else if (ft_strchr(OCTAL, data->type) != NULL)
		make_octal_conv(&res, data, ap);
	else if (ft_strchr(HEX, data->type) != NULL)
		make_hex_conv(&res, data, ap);
	else if (ft_strchr(CHAR, data->type) != NULL)
		make_char_conv(&res, data, ap);
	else if (ft_strchr(STR, data->type) != NULL)
		make_str_conv(&res, data, ap);
	else if (data->type == 'p')
		make_ptr_conv(&res, data, ap);
	else if (data->type == '0')
		make_no_type_conv(&res, data);
	else
		return (make_conv2(data, ap));
	return (res);
}

void	free_struct(t_part *part)
{
	if (part != NULL)
	{
		if (part->conv != NULL)
		{
			if (part->conv->flags != NULL)
				free(part->conv->flags);
			if (part->conv->mod != NULL)
				free(part->conv->mod);
			free(part->conv);
		}
		if (part->res != NULL)
			free(part->res);
		free(part);
	}
}

int		print_chain(t_part *wlst, int fd)
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
		if (wlst->conv && wlst->conv->type == 'n')
			get_n_value(ret, wlst->conv);
		else if ((wlst->next != NULL && wlst->next->res == NULL
			&& wlst->next->conv->type != 'n') || wlst->res == NULL)
			dead = 1;
		if (dead != 1)
			write(fd, wlst->res, retsum);
		ret += retsum;
		tmp = wlst->next;
		free_struct(wlst);
		wlst = tmp;
	}
	return (dead == 1 ? -1 : ret);
}

int		ft_printf(const char *format, ...)
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
	ret = print_chain(tmp, 1);
	return (ret);
}
