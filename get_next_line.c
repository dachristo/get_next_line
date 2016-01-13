/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchristo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/21 16:12:17 by dchristo          #+#    #+#             */
/*   Updated: 2016/01/13 17:05:28 by dchristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "get_next_line.h"

static int		in_while(char **tmp, int *boucle, char **line, int i)
{
	tmp[0][i] = '\0';
	*boucle = *boucle + 1;
	ft_strcpy(tmp[1], *line);
	free(*line);
	if ((*line = ft_strnew((*(boucle) + 1) * BUFF_SIZE)) == NULL)
		return (-1);
	ft_strcpy(*line, tmp[1]);
	free(tmp[1]);
	if ((tmp[1] = ft_strnew((*(boucle) + 1) * BUFF_SIZE)) == NULL)
		return (-1);
	ft_strcat(*line, tmp[0]);
	return (0);
}

static int		recup_save_backline(char **line, int i, char *save, char *buf)
{
	int j;

	if ((buf = ft_strnew(BUFF_SIZE)) == NULL)
		return (-1);
	j = 0;
	while (save[++i] != '\n' && save[i] != '\0')
		buf[i] = save[i];
	buf[i] = '\0';
	ft_strcpy(*line, buf);
	if (save[i] == '\n')
	{
		while (save[i++] != '\0')
			save[j++] = save[i];
		return (1);
	}
	return (0);
}

static int		retour(int i, char *buf, char *save, char **line)
{
	int		j;

	j = 0;
	if (i == -1)
		return (-1);
	else if (buf[i] == '\n')
	{
		while (buf[++i])
			save[j++] = buf[i];
		save[j] = '\0';
		return (1);
	}
	else if (save[0] != '\0' && i == 0)
	{
		i--;
		if (recup_save_backline(line, i, save, buf))
			return (1);
	}
	return (0);
}

static int		init(char ***tmp, char **line, char **save)
{
	if (!line)
		return (-1);
	if ((*tmp = (char **)malloc(sizeof(char *) * 3)) == NULL)
		return (-1);
	if ((*line = ft_strnew(BUFF_SIZE)) == NULL)
		return (-1);
	if (!*save)
	{
		if ((*save = ft_strnew(BUFF_SIZE)) == NULL)
			return (-1);
	}
	else
	{
		if (recup_save_backline(line, -1, *save, NULL))
			return (1);
	}
	return (0);
}

int				get_next_line(int const fd, char **line)
{
	char				**tmp;
	static	char		*save[256];
	int					i;
	int					boucle;

	boucle = 0;
	if ((i = init(&tmp, line, &save[fd])) != 0)
		return (i);
	i = -1;
	while (++i < 3)
		if ((tmp[i] = (char *)malloc((sizeof(char) * BUFF_SIZE + 1))) == NULL)
			return (-1);
	while ((i = read(fd, tmp[2], BUFF_SIZE)) > 0)
	{
		tmp[2][i] = '\0';
		i = -1;
		while (tmp[2][++i] && tmp[2][i] != '\n')
			tmp[0][i] = tmp[2][i];
		if ((in_while(tmp, &boucle, line, i)) == -1)
			return (-1);
		if (tmp[2][i] == '\n')
			break ;
	}
	return (retour(i, tmp[2], save[fd], line));
}
