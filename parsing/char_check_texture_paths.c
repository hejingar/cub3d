/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_check_texture_paths.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:28:50 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/22 21:42:34 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

void	default_textures_paths_if_necessary(t_map *map)
{
	if (map->north == NULL)
		map->north = ft_strdup("./textures/red_bricks_dark.xpm");
	if (map->south == NULL)
		map->south = ft_strdup("./textures/red_bricks_bright.xpm");
	if (map->east == NULL)
		map->east = ft_strdup("./textures/red_bricks.xpm");
	if (map->west == NULL)
		map->west = ft_strdup("./textures/red_bricks.xpm");
}

int	grid_first_line(char **content)
{
	int	i;
	int	j;

	i = 0;
	while (content[i] != NULL)
	{
		j = 0;
		while (content[i][j] != '\0' && (content[i][j] == ' '
			|| content[i][j] == '1'))
			j++;
		if (j != 0 && content[i][j] == '\0')
			return (i);
		i++;
	}
	return (-1);
}

void	grid_char_check(char **c, t_map *map, int i, int j)
{
	int	player;

	map->grid_start = grid_first_line(c);
	if (map->grid_start < 0)
		parse_error(4, map);
	i = map->grid_start;
	player = 0;
	while (c[i] != NULL)
	{
		j = 0;
		while (c[i][j] != '\0' && (c[i][j] == ' ' || c[i][j] == '1'
					|| c[i][j] == '0' || c[i][j] == 'N' || c[i][j] == 'S'
					|| c[i][j] == 'W' || c[i][j] == 'E'))
		{
			if (c[i][j] == 'N' || c[i][j] == 'S' || c[i][j] == 'E'
					|| c[i][j] == 'W')
				player++;
			j++;
		}
		if (c[i][j] != '\0' || c[i][0] == '\0')
			parse_error(4, map);
		i++;
	}
	if (player != 1)
		parse_error(4, map);
}

char	*copy_texture_path(char *str, t_map *map, int i, char *res)
{
	while (str[i] != '\0' && str[i] != ' ')
		i++;
	res = malloc(sizeof(char) * (i + 1));
	if (res == NULL)
		parse_error(5, map);
	if (str[i] == ' ')
	{
		while (str[i] == ' ')
			i++;
		if (str[i] != '\0')
		{
			free(res);
			parse_error(4, map);
		}
	}
	i = 0;
	while (str[i] != '\0' && str[i] != ' ')
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*get_texture_path(char **content, char *dir, int i, t_map *map)
{
	int	j;

	j = 0;
	while (content[i] != NULL)
	{
		if (ft_strnstr(content[i], dir, ft_strlen(content[i])) != NULL)
			break ;
		i++;
	}
	if (content[i] == NULL)
		return (NULL);
	while (content[i][j] == ' ')
		j++;
	if (content[i][j] != dir[0] || content[i][j + 1] != dir[1]
		|| content[i][j + 2] != ' ')
		parse_error(4, map);
	j = j + 2;
	while (content[i][j] == ' ')
		j++;
	if (content[i][j] != '\0')
		return (copy_texture_path(&content[i][j], map, 0, NULL));
	return (NULL);
}
