/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minilibx_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-youb <ael-youb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 14:53:57 by ael-youb          #+#    #+#             */
/*   Updated: 2023/03/23 14:57:14 by ael-youb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (x * data->ll + y * (data->bpp / 8));
	*(unsigned int *)dst = color;
}

void	draw_player(t_game *game)
{
	int	i;
	int	j;

	i = 75; //pix * nombre de cases minimap (ici -5 a +5)
	j = 75;
	while (i < 79)
	{
		while (j < 79)
		{
			pixel_put(game->img, i, j,
				0x80090888);
			j++;
		}
		j = 75;
		i++;
	}
	i = 75 + game->player_deltax;
	j = 75 + game->player_deltay;
	while (i < 78 + game->player_deltax)
	{
		while (j < 78 + game->player_deltay)
		{
			pixel_put(game->img, j, i,
				0x00FF0000);
			j++;
		}
		j = 75 + game->player_deltay;
		i++;
	}
}

float	dist(float ax, float ay, float bx, float by)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void	loop_draw_three_d(t_game *game, float height, float offset)
{
	int		j;

	j = 0;
	while (j < offset)
	{
		pixel_put(game->img, j, (230 + game->store->r),
			0x0ADD8E6);
			j++;
	}
	while (j < height + offset)
	{
		if (game->store->color)
			pixel_put(game->img, j, (230 + game->store->r),
				0x00002888);
		else
			pixel_put(game->img, j, (230 + game->store->r),
				0x80090888);
		j++;
	}
	j = offset;
}

void	draw_three_d(t_game *game, float distance, float ra)
{
	float	height;
	float	offset;
	float	plan_fisheye;

	plan_fisheye = game->player_angle - ra;
	if (plan_fisheye < 0)
		plan_fisheye += 2 * PI;
	if (plan_fisheye > 2 * PI)
		plan_fisheye -= 2 * PI;
	distance = distance * cos(plan_fisheye);
	height = (8 * 2 * 512) / distance; //bizarre value
	if (height > 512)
		height = 512;
	offset = 200 - height / 2;
	loop_draw_three_d(game, height, offset);
}

void	add_to_image(t_game *game)
{
	float	i;
	float	j;

	i = 0;
	j = 0;
	while (i < 512)
	{
		while (j < 1024)
		{
			pixel_put(game->img, i, j,
				0x80808080);
			j++;
		}
		j = 0;
		i++;
	}
	draw_ray(game);
	draw_map(game);
	draw_player(game);
	mlx_put_image_to_window(game->win->mlx, game->win->mlx_win, game->img->img, 0, 0);
}

void	draw_map(t_game *game)
{
	int	x;
	int	y;
	int	x_origin;
	int	y_origin;
	int	xo;
	int	yo;
	int	pix;

	x = (int)(game->player_x) / (512 / game->map.grid_height);
	y = (int)(game->player_y) / (512 / game->map.grid_height);
	x = x - 5;
	y = y - 5;
	x_origin = 0;
	y_origin = 0;
	pix = 512 / (game->map.grid_width);
	while (x < (int)(game->player_x) / (512 / game->map.grid_height) + 5)
	{
		while (y < (int)(game->player_y) / (512 / game->map.grid_height) + 5)
		{
			xo = x_origin * pix;
			yo = y_origin * pix;
			while (xo < ((x_origin + 1) * pix) - 1) // 1px offset entre chaque case
			{
				while (yo < ((y_origin + 1) * pix) - 1)
				{
					if (x < 0 || x >= game->map.grid_height)
					{
						break ;
					}
					if (y < 0 || y >= game->map.grid_height)
					{
						//printf("breaking : %d %d\n", y, x);
						break ;
					}
					//printf("drawing to pixel %d %d\npix%d\n", yo, xo, pix);
					pixel_put(game->img, yo, xo,
						0x00000000);
					if (game->map.grid[y][x] == '1')
					{
						pixel_put(game->img, yo, xo,
							0x88800000);
					}
					else
					{
						pixel_put(game->img, yo, xo,
							0x99999999);
					}
					yo++;
				}
				yo = y_origin * pix;
				xo++;
			}
			//printf("drawn : %d %d\n", y, x);
			y++;
			y_origin++;
		}
		x++;
		x_origin++;
		y_origin = 0;
		y = (int)(game->player_y) / (512 / game->map.grid_height);
		y = y - 5;
	}
}

void	init_mlx(t_game *game)
{
	t_vars	win;
	t_data	img;

	win.mlx = mlx_init();
	win.mlx_win = mlx_new_window(win.mlx, 1024, 512, "Louveteau 3D");
	img.img = mlx_new_image(win.mlx, 1024, 512);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.ll, &img.endian);
	game->win = &win;
	game->img = &img;
	add_to_image(game);
	mlx_put_image_to_window(win.mlx, win.mlx_win, game->img->img, 0, 0);
	mlx_hook(win.mlx_win, 2, 1L << 0, hook_slide, game);
	mlx_hook(win.mlx_win, 17, 0L, destroy_window, game);
	mlx_loop(win.mlx);
}
