/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minilibx_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
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

int	get_colour_from_texture(int height, t_data *data, t_game *game, int wall_y)
{
	char	*dst;
	int	x;
	int	y;
	unsigned int res;
	float		tmp;

	if (game->store.distance == game->store.dis_h)
		tmp = game->store.rx / (512 / (game->map.grid_width));
	else
		tmp = game->store.ry / (512 / (game->map.grid_width));
	tmp -= (int)tmp;
	tmp *= 1000;
	y = (int)tmp;
	y = data->w * y / 1000;
	if (height == game->store.actual_height)
		x = wall_y * data->h / height;
	else
		x = (wall_y + ((game->store.actual_height - height) / 2)) * data->h / game->store.actual_height;
	dst = data->addr + (x * data->ll + y * (data->bpp /8));
	res = *(unsigned int *)dst;
	return (res);
}

void	loop_draw_three_d(t_game *game, float height, float offset)
{
	int		j;
	float		px;
	float		py;

	j = 0;
	while (j < offset)
	{
		pixel_put(game->img, j, (230 + game->store->r),
			0x0ADD8E6);
			j++;
	px = (game->player_x) / (512 / game->map.grid_height);
	py = (game->player_y) / (512 / game->map.grid_height);
	while (j < height + offset)
	{
		if (game->store.distance == game->store.dis_h && py > game->store.my)
			pixel_put(game->img, j, 530 + game->store.r,
			get_colour_from_texture(height, &game->south, game, j - offset));
		else if (game->store.distance == game->store.dis_h && py < game->store.my)
			pixel_put(game->img, j, 530 + game->store.r,
			get_colour_from_texture(height, &game->north, game, j - offset));
		else if (game->store.distance == game->store.dis_v && px > game->store.mx)
			pixel_put(game->img, j, 530 + game->store.r,
			get_colour_from_texture(height, &game->west, game, j - offset));
		else if (game->store.distance == game->store.dis_v && px < game->store.mx)
			pixel_put(game->img, j, 530 + game->store.r,
			get_colour_from_texture(height, &game->east, game, j - offset));
		j++;
    }
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
	game->store.actual_height = height;
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
	draw_map(game);
	draw_player(game);
	draw_rays(game);
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

void	load_textures(t_game *game)
{
	game->north.img = mlx_xpm_file_to_image(game->win->mlx, game->map.north,
		&game->north.w, &game->north.h);
	game->south.img = mlx_xpm_file_to_image(game->win->mlx, game->map.south,
		&game->south.w, &game->south.h);
	game->east.img = mlx_xpm_file_to_image(game->win->mlx, game->map.east,
		&game->east.w, &game->east.h);
	game->west.img = mlx_xpm_file_to_image(game->win->mlx, game->map.west,
		&game->west.w, &game->west.h);
	if (game->north.img == NULL || game->south.img == NULL
		|| game->east.img == NULL || game->west.img == NULL)
	{
		ft_putendl_fd("Error\nTextures couldn't be loaded", 2);
		close_program(game);
	}
	game->north.addr = mlx_get_data_addr(game->north.img, &game->north.bpp,
		&game->north.ll, &game->north.endian);
	game->south.addr = mlx_get_data_addr(game->south.img, &game->south.bpp,
		&game->south.ll, &game->south.endian);
	game->east.addr = mlx_get_data_addr(game->east.img, &game->east.bpp,
		&game->east.ll, &game->east.endian);
	game->west.addr = mlx_get_data_addr(game->west.img, &game->west.bpp,
		&game->west.ll, &game->west.endian);
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
	load_textures(game);
	add_to_image(game);
	mlx_put_image_to_window(win.mlx, win.mlx_win, game->img->img, 0, 0);
	mlx_hook(win.mlx_win, 2, 1L << 0, hook_slide, game);
	mlx_hook(win.mlx_win, 17, 0L, destroy_window, game);
	mlx_loop(win.mlx);
}
