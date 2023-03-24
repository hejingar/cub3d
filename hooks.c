/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-youb <ael-youb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:04:14 by ael-youb          #+#    #+#             */
/*   Updated: 2023/03/22 15:47:49 by ael-youb         ###   ########.fr       */
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

	mx = (int)(game->player_x - (game->player_deltax * 2)) / (512 / game->map.grid_height);
	my = (int)(game->player_y - (game->player_deltay * 2)) / (512 / game->map.grid_height);
	if (mx < game->map.grid_height && mx > 0)
	{
		if (my < game->map.grid_height && my > 0)
		{
			if (game->map.grid[my][mx] == '0')
			{
				game->player_x -= game->player_deltax;
				game->player_y -= game->player_deltay;
			}
		}
	}
}

void	go_forward(t_game *game)
{
	int	mx;
	int	my;

	mx = (int)(game->player_x + (game->player_deltax * 2)) / (512 / game->map.grid_height);
	my = (int)(game->player_y + (game->player_deltay * 2)) / (512 / game->map.grid_height);
	if (mx < game->map.grid_height && mx > 0)
	{
		if (my < game->map.grid_height && my > 0)
		{
			if (game->map.grid[my][mx] == '0')
			{
				game->player_x += game->player_deltax;
				game->player_y += game->player_deltay;
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
		go_backward(game);
	else if (keycode == 65362)
		go_forward(game);
	add_to_image(game);
	return (keycode);
}

void	close_program(t_game *game)
{
	mlx_destroy_window(game->win->mlx, game->win->mlx_win);
	mlx_destroy_image(game->win->mlx, game->img->img);
	if (game->north.img != NULL)
		mlx_destroy_image(game->win->mlx, game->north.img);
	if (game->south.img != NULL)
		mlx_destroy_image(game->win->mlx, game->south.img);
	if (game->east.img != NULL)
		mlx_destroy_image(game->win->mlx, game->east.img);
	if (game->west.img != NULL)
		mlx_destroy_image(game->win->mlx, game->west.img);
	mlx_destroy_display(game->win->mlx);
	free(game->win->mlx);
	free_map(&game->map);
	//free(game);

	exit(1);
}
