/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:11:53 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:11:54 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void*, size_t))
{
	t_list	*tmp;
	t_list	*ntmp;

	tmp = *alst;
	if (tmp->next != NULL)
		ntmp = tmp->next;
	else
		ntmp = NULL;
	while (ntmp != NULL)
	{
		(*del)(tmp->content, tmp->content_size);
		tmp = ntmp;
		ntmp = tmp->next;
		free(tmp);
	}
	(*del)(tmp->content, tmp->content_size);
	ft_memdel((void*)alst);
}
