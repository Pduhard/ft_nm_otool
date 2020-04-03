/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_conv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:07:27 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:07:30 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	make_no_type_conv(char **converted, t_conv *data)
{
	char	*tmp;

	if ((tmp = ft_strchr(data->flags, '0')) != NULL
			&& ft_strchr(data->flags, '-') != NULL)
		*tmp = '9';
	*converted = ft_strfillnew(data->no_type, 1);
	display_str_flags(converted, data);
	data->size = ft_strlen(*converted);
}

void	make_char_conv(char **converted, t_conv *data, va_list ap)
{
	char			c;
	unsigned int	u;
	int				len;

	*converted = NULL;
	if ((data->type == 'c' && data->mod &&
				ft_strcmp(data->mod, "l") == 0) || data->type == 'C')
	{
		u = va_arg(ap, unsigned int);
		*converted = wchart_conv(u, data, &len);
		if (*converted)
			display_str_flags(converted, data);
		if (*converted)
			data->size = display_bsz(data, u, converted);
	}
	else
	{
		c = va_arg(ap, int);
		*converted = ft_strfillnew(c, 1);
		display_str_flags(converted, data);
		data->size = display_bsz(data, c, converted);
	}
}

char	*null_string(char **str)
{
	ft_strdel(str);
	*str = ft_strdup("(null)");
	return (*str);
}

char	*unistr_conv(wchar_t *unistr, t_conv *data)
{
	int		i;
	char	*tmp;
	char	*str;
	int		len;
	int		prec_count;

	i = 0;
	prec_count = 0;
	str = ft_strnew(0);
	while (unistr && unistr[i] != '\0')
	{
		tmp = wchart_conv(unistr[i++], data, &len);
		prec_count += len;
		if (data->prec != -1 && prec_count > data->prec)
		{
			ft_strdel(&tmp);
			return (str);
		}
		if (tmp == NULL)
			ft_strdel(&str);
		if (tmp == NULL)
			return (NULL);
		str = ft_strfjoin(str, tmp);
	}
	return (unistr == NULL ? null_string(&str) : str);
}

void	make_str_conv(char **converted, t_conv *data, va_list ap)
{
	char *tmp;

	if ((tmp = ft_strchr(data->flags, '0'))
			!= NULL && ft_strchr(data->flags, '-'))
		*tmp = '9';
	*converted = NULL;
	if ((data->type == 's' && data->mod &&
		ft_strcmp(data->mod, "l") == 0) || data->type == 'S')
		*converted = unistr_conv(va_arg(ap, wchar_t *), data);
	else
	{
		*converted = ft_strdup(va_arg(ap, char *));
		if (*converted == NULL)
			*converted = ft_strdup("(null)");
	}
	if (*converted != NULL)
	{
		display_str_flags(converted, data);
		data->size = ft_strlen(*converted);
	}
}
