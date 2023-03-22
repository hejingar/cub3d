/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 23:20:40 by ael-youb          #+#    #+#             */
/*   Updated: 2023/03/22 19:38:14 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

//attention, y et x inverses dans map
void	loop_distance_h(t_game *game, int pix)
{
	while (game->store.dof < 8)
	{
		game->store.mx = (int)(game->store.rx) / pix;
		game->store.my = (int)(game->store.ry) / pix;
		game->store.mp = game->store.my
			* game->map.grid_height + game->store.mx;
		if (game->store.mp > 0
			&& game->store.mp < game->map.grid_height * game->map.grid_height
			&& game->map.grid[game->store.my][game->store.mx] == '1')
		{
			game->store.hx = game->store.rx;
			game->store.hy = game->store.ry;
			game->store.dis_h = dist(game->player_x, game->player_y,
					game->store.hx, game->store.hy);
			game->store.dof = 8;
		}
		else
		{
			game->store.rx += game->store.xo;
			game->store.ry += game->store.yo;
			game->store.dof++;
		}
	}
}

// check horizontal lines
void	distance_h(t_game *game, int pix)
{
	game->store.dof = 0;
	if (game->store.ra > PI)
	{
		game->store.ry = (((int)game->player_y / pix) * pix) - 0.0001;
		game->store.rx = (game->player_y - game->store.ry)
			* (-1 / tan(game->store.ra)) + game->player_x;
		game->store.yo = -pix;
		game->store.xo = -game->store.yo * (-1 / tan(game->store.ra));
	}
	if (game->store.ra < PI)
	{
		game->store.ry = (((int)game->player_y / pix) * pix) + pix;
		game->store.rx = (game->player_y - game->store.ry)
			* (-1 / tan(game->store.ra)) + game->player_x;
		game->store.yo = pix;
		game->store.xo = -game->store.yo * (-1 / tan(game->store.ra));
	}
	if (game->store.ra == PI || game->store.ra == 0)
	{
		game->store.rx = game->player_x;
		game->store.ry = game->player_y;
		game->store.dof = 8;
	}
	loop_distance_h(game, pix);
}

void	loop_distance_v(t_game *game, int pix)
{
	while (game->store.dof < 8)
	{
		game->store.mx = (int)(game->store.rx) / pix;
		game->store.my = (int)(game->store.ry) / pix;
		game->store.mp = game->store.my
			* game->map.grid_height + game->store.mx;
		if (game->store.mp > 0
			&& game->store.mp < game->map.grid_height * game->map.grid_height
			&& game->map.grid[game->store.my][game->store.mx] == '1')
		{
			game->store.vx = game->store.rx;
			game->store.vy = game->store.ry;
			game->store.dis_v = dist(game->player_x, game->player_y,
					game->store.vx, game->store.vy);
			game->store.dof = 8;
		}
		else
		{
			game->store.rx += game->store.xo;
			game->store.ry += game->store.yo;
			game->store.dof++;
		}
	}
}

// check vertical lines
void	distance_v(t_game *game, int pix)
{
	game->store.dof = 0;
	if (game->store.ra > P2 && game->store.ra < P3)
	{
		game->store.rx = (((int)game->player_x / pix) * pix) - 0.0001;
		game->store.ry = (game->player_x - game->store.rx)
			* (-tan(game->store.ra)) + game->player_y;
		game->store.xo = -pix;
		game->store.yo = -game->store.xo * (-tan(game->store.ra));
	}
	if (game->store.ra < P2 || game->store.ra > P3)
	{
		game->store.rx = (((int)game->player_x / pix) * pix) + pix;
		game->store.ry = (game->player_x - game->store.rx)
			* (-tan(game->store.ra)) + game->player_y;
		game->store.xo = pix;
		game->store.yo = -game->store.xo * (-tan(game->store.ra));
	}
	if (game->store.ra == PI || game->store.ra == 0)
	{
		game->store.rx = game->player_x;
		game->store.ry = game->player_y;
		game->store.dof = 8;
	}
	loop_distance_v(game, pix);
}

void	draw_rays(t_game *game)
{
	int	pix;
	int	wall_start;
	int	wall_x;
	int	wall_y;

	pix = 512 / game->map.grid_height;
	init_draw_ray(game);
	while (game->store.r < RAY_NUMBER)
	{
		wall_x = game->store.mx;
		wall_y = game->store.my;
		wall_start = game->store.r;
		game->store.wall_ray_nb = 0;
		game->store.wall_width = 0;
		while (game->store.r < RAY_NUMBER && wall_x == game->store.mx && wall_y == game->store.my)
		{
			distance_h(game, pix);
			distance_v(game, pix);
			pick_v_or_h(game, pix);
			game->store.r++;
			game->store.ra += (DR / 8);
			if (game->store.ra < 0)
				game->store.ra += 2 * PI;
			if (game->store.ra > 2 * PI)
				game->store.ra -= 2 * PI;
			game->store.wall_width++;
		}
		game->store.mx = wall_x;
		game->store.my = wall_y;
		while (game->store.r > wall_start)
		{
			game->store.ra -= (DR / 8);
			if (game->store.ra < 0)
				game->store.ra += 2 * PI;
			if (game->store.ra > 2 * PI)
				game->store.ra -= 2 * PI;
			game->store.r--;
		}
		while (game->store.r < RAY_NUMBER && wall_x == game->store.mx && wall_y == game->store.my)
		{
			distance_h(game, pix);
			distance_v(game, pix);
			pick_v_or_h(game, pix);
			draw_minimap_dot(game);
			game->store.r++;
			game->store.wall_ray_nb++;
			draw_three_d(game, game->store.distance, game->store.ra);
			game->store.ra += (DR / 8);
			if (game->store.ra < 0)
				game->store.ra += 2 * PI;
			if (game->store.ra > 2 * PI)
				game->store.ra -= 2 * PI;
		}
	}
}