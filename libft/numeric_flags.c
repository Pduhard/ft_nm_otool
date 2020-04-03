/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_flags.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:24:46 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:24:46 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		display_prec(char **converted, int prec, int *len)
{
	int		modified;
	char	*tmp;
	char	*tmp2;

	if (prec > (*len = (int)ft_strlen(*converted)))
	{
		tmp = ft_strfillnew('0', prec - *len);
		tmp2 = *converted;
		*converted = ft_strjoin(tmp, tmp2);
		ft_strdel(&tmp);
		ft_strdel(&tmp2);
		modified = 1;
	}
	else
		modified = 0;
	return (modified);
}

void	display_space(char **converted, char *flags, int modified)
{
	int		i;
	char	*tmp;

	if (ft_strchr(flags, ' ') != NULL)
	{
		i = 0;
		if (ft_strchr(*converted, ' ') == NULL && modified == 1)
		{
			while (*converted[i] && *converted[i] != '0')
				i++;
			*converted[i] = ' ';
		}
		else if (ft_strchr(*converted, ' ') == NULL)
		{
			tmp = *converted;
			*converted = ft_strjoin(" ", tmp);
			ft_strdel(&tmp);
		}
	}
}

int		make_c_join(char *c, char **converted, int ret)
{
	char	*tmp;

	tmp = *converted;
	*converted = ft_strjoin(c, tmp);
	ft_strdel(&tmp);
	return (ret);
}

int		display_width(int zero, int *neg, t_conv *data, char **converted)
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;
	char	*tmp4;
	int		l;

	if (data->width > (l = (int)ft_strlen(*converted)) + *neg)
	{
		if ((tmp4 = ft_strchr(data->flags, '+')) != NULL && *neg != 1)
			l++;
		if ((tmp3 = ft_strchr(data->flags, '0')) != NULL && zero == 1
				&& data->prec == -1)
			tmp = ft_strfillnew('0', data->width - l - *neg);
		else
			tmp = ft_strfillnew(' ', data->width - l - *neg);
		if (*neg == 1 && tmp3 == NULL && tmp[0] == ' ')
			*neg = make_c_join("-", converted, 2);
		else if (tmp4 != NULL && *neg != 1 && tmp[0] == ' ')
			*neg = make_c_join("+", converted, 2);
		tmp2 = *converted;
		*converted = (ft_strchr(data->flags, '-') != NULL) ?
			ft_strfjoin(tmp2, tmp) : ft_strfjoin(tmp, tmp2);
		return (1);
	}
	return (0);
}

int		check_zero(char **converted, t_conv *data)
{
	int		zero;
	char	*hash;

	zero = 0;
	if (ft_atoi(*converted) == 0)
	{
		if (data->prec == 0)
		{
			free(*converted);
			*converted = ft_strnew(0);
		}
		else
			zero = 1;
		if ((hash = ft_strchr(data->flags, '#')) != NULL
				&& (data->type == 'x' || data->type == 'X'))
			*hash = '9';
	}
	return (zero);
}
