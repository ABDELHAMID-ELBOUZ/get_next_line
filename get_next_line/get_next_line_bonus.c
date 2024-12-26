/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 11:19:23 by aelbouz           #+#    #+#             */
/*   Updated: 2024/12/26 08:33:44 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

ssize_t	r_line(int fd, char **str)
{
	char	*buff;
	char	*tmp;
	ssize_t	r_size;

	buff = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buff)
		return (free(*str), *str = NULL, -1);
	r_size = read(fd, buff, BUFFER_SIZE);
	if (r_size <= 0)
		return (free(buff), r_size);
	buff[r_size] = '\0';
	tmp = ft_strjoin(*str, buff);
	if (!tmp)
		return (free(*str), free(buff), *str = NULL, -1);
	free(*str);
	free(buff);
	*str = tmp;
	return (r_size);
}

char	*rm_overflow(char *str)
{
	int		i;
	char	*result;

	if (!str)
		return (ft_strdup(str));
	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	result = malloc((i + 1) * sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, str, i + 1);
	return (result);
}

char	*start_next_line(char *str)
{
	int		i;
	char	*result;

	i = 0;
	if (!str[i])
		return (free(str), NULL);
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	result = ft_strdup(str + i);
	if (!result)
		return (free(str), NULL);
	free(str);
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*str[FD_SETSIZE];
	char		*result;
	ssize_t		r_size;

	if (read(fd, NULL, 0) < 0)
		return (free(str[fd]), str[fd] = NULL, NULL);
	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= INT_MAX
		|| fd > FD_SETSIZE)
		return (NULL);
	while (!ft_strrchr(str[fd], '\n'))
	{
		r_size = r_line(fd, &str[fd]);
		if (r_size < 0)
			return (free(str[fd]), str[fd] = NULL, NULL);
		if (r_size == 0)
			break ;
	}
	if (!str[fd] || !*str[fd])
		return (free(str[fd]), str[fd] = NULL, NULL);
	result = rm_overflow(str[fd]);
	if (!result)
		return (free(str[fd]), str[fd] = NULL, NULL);
	str[fd] = start_next_line(str[fd]);
	return (result);
}
