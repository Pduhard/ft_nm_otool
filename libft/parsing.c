/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:25:09 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:25:09 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*get_conv_string(char *str, int *len)
{
	int		j;
	char	*string;

	j = 0;
	while (str[j] && (ft_strchr(VALID, str[j]) != NULL || ft_isdigit(str[j])))
	{
		if (ft_strchr(TYPES, str[j]) != NULL)
			break ;
		j++;
	}
	j++;
	string = ft_strsub(str, 0, j);
	*len = j;
	return (string);
}

t_conv	*parse_conv(char *str, int *i, va_list ap)
{
	t_conv	*conv;
	char	*string;
	int		len;

	if (!(conv = (t_conv *)malloc(sizeof(*conv))))
		return (NULL);
	string = get_conv_string((str + *i + 1), &len);
	conv->flags = get_flags(string, 0);
	conv->width = get_width(string, 0, ap, &(conv->flags));
	conv->prec = get_prec(string, len - 1, ap);
	conv->mod = get_mod(string);
	conv->type = get_type(string[len - 1], conv);
	conv->no_type = conv->type == '0' ? str[*i + len] : '%';
	*i += str[*i + len] ? len + 1 : len;
	conv->type = conv->type == '%' ? '0' : conv->type;
	conv->size = 0;
	conv->n_conv = NULL;
	ft_strdel(&string);
	return (conv);
}

t_part	*get_next_part(char *str, int *i, int j, va_list ap)
{
	t_part	*part;

	if (!(part = (t_part *)malloc(sizeof(*part))))
		return (NULL);
	if (str[*i] == '%')
	{
		if (!(part->conv = parse_conv(str, i, ap)))
			return (NULL);
		part->res = NULL;
	}
	else
	{
		while (str[j] && str[j] != '%')
			j++;
		if (!(part->res = (char *)malloc(j + 1)))
			return (NULL);
		j = 0;
		while (str[*i] && str[*i] != '%')
			part->res[j++] = str[(*i)++];
		part->res[j] = '\0';
		part->conv = NULL;
	}
	part->next = NULL;
	return (part);
}

t_part	*parse_format(char *format, va_list ap)
{
	t_part	*begin;
	t_part	*lst;
	int		i;

	i = 0;
	begin = NULL;
	while (format[i])
	{
		if (begin == NULL)
		{
			lst = get_next_part(format, &i, i, ap);
			begin = lst;
		}
		else
		{
			lst->next = get_next_part(format, &i, i, ap);
			lst = lst->next;
		}
	}
	return (begin);
}
