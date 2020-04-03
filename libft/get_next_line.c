/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <pduhard-@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:23:20 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 02:23:20 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		get_rest(char **s, char **line)
{
	char	*tmp;
	char	*str;
	int		i;

	str = *s;
	i = 0;
	while (str[i] != '\n' && str[i] != '\0')
		i++;
	if (str[i] == '\0')
		return (0);
	*line = ft_strsub(str, 0, i);
	tmp = ft_strdup(&str[i + 1]);
	ft_strdel(s);
	*s = tmp;
	return (1);
}

static t_mfd	*get_corr_fd(int fd, t_mfd *mfd)
{
	t_mfd	*tmp;

	if (mfd != NULL)
	{
		while (mfd->next != NULL)
		{
			if (mfd->fd == fd)
				return (mfd);
			mfd = mfd->next;
		}
		if (mfd->fd == fd)
			return (mfd);
	}
	if (!(tmp = malloc(sizeof(*tmp))))
		return (NULL);
	tmp->fd = fd;
	tmp->str = ft_strnew(0);
	tmp->next = NULL;
	if (mfd != NULL)
		mfd->next = tmp;
	return (tmp);
}

static int		make_clean(t_mfd **mfd, t_mfd *beg, t_mfd **ffd)
{
	t_mfd	*tmp;

	tmp = *mfd;
	if (beg == tmp)
		*ffd = tmp->next;
	if (beg != tmp)
	{
		while (beg->next != tmp)
			beg = beg->next;
		beg->next = beg->next->next;
	}
	ft_strdel(&tmp->str);
	tmp->fd = 0;
	tmp->next = NULL;
	free(tmp);
	tmp = NULL;
	return (0);
}

static char		*make_strjoin(char **str, char *buf, int ret)
{
	char *res;
	char *tmp;

	if (ret != -2)
		buf[ret] = '\0';
	tmp = *str;
	res = ft_strjoin(tmp, buf);
	ft_strdel(&tmp);
	return (res);
}

int				get_next_line(const int fd, char **line)
{
	int				ret;
	char			buf[BUFF_SIZE + 1];
	static t_mfd	*ffd = NULL;
	t_mfd			*mfd;

	if (fd < 0 || line == NULL || BUFF_SIZE <= 0)
		return (-1);
	if (ffd == NULL)
		ffd = get_corr_fd(fd, ffd);
	mfd = get_corr_fd(fd, ffd);
	if (get_rest(&mfd->str, line) == 1)
		return (1);
	while ((ret = read(fd, buf, BUFF_SIZE)) && ret > 0)
	{
		mfd->str = make_strjoin(&mfd->str, buf, ret);
		if (get_rest(&mfd->str, line) == 1)
			return (1);
	}
	if (ft_strlen(mfd->str) > 0)
	{
		mfd->str = make_strjoin(&mfd->str, "\n", -2);
		return (get_rest(&mfd->str, line));
	}
	return ((ret == -1) ? -1 : make_clean(&mfd, ffd, &ffd));
}
