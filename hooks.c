/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:04:14 by ael-youb          #+#    #+#             */
/*   Updated: 2023/03/21 15:39:20 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	destroy_window(t_game *game)
{
	close_program(game);
	return (1);
}

void	turn_left(t_game *game)
{
	game->player_angle -= 0.1;
	if (game->player_angle < 0)
		game->player_angle+= 2*PI;
	game->player_deltax = cos(game->player_angle) * 5;
	game->player_deltay = sin(game->player_angle) * 5;
}

void	turn_right(t_game *game)
{
	game->player_angle += 0.1;
	if (game->player_angle > 2*PI)
		game->player_angle-= 2*PI;
	game->player_deltax = cos(game->player_angle) * 5;
	game->player_deltay = sin(game->player_angle) * 5;
}

void	go_backward(t_game *game)
{
	int	mx;
	int	my;

	mx = (int)game->player_x / game->map.grid_height;
	my = (int)game->player_y / game->map.grid_height;
	if (mx < game->map.grid_height && mx > 0)
	{
		if (my < game->map.grid_height && my > 0)
		{
			printf("%d:%d\n", mx, my);
			if (!game->map.grid[my][mx])
			{
				game->player_x -= game->player_deltax;
				game->player_y -= game->player_deltay;
			}
		}
	}
}

int	hook_slide(int keycode, t_game *game)
{
	if (keycode == 65361)
		turn_left(game);
	else if (keycode == 65363)
		turn_right(game);
	else if (keycode == 65364)
	{
		game->player_x -= game->player_deltax;
		game->player_y -= game->player_deltay;
	}
	else if (keycode == 65362)
	{
		game->player_x += game->player_deltax;
		game->player_y += game->player_deltay;
	}
	add_to_image(game);
	return (keycode);
}

void	close_program(t_game *game)
{
	mlx_destroy_window(game->win->mlx, game->win->mlx_win);
	mlx_destroy_image(game->win->mlx, game->img->img);
	mlx_destroy_image(game->win->mlx, game->north.img);
	mlx_destroy_image(game->win->mlx, game->south.img);
	mlx_destroy_image(game->win->mlx, game->east.img);
	mlx_destroy_image(game->win->mlx, game->west.img);
	mlx_destroy_display(game->win->mlx);
	free(game->win->mlx);
	free(game->store);
	free_map(&game->map);
	//free(game);

	exit(1);
}
