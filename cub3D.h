	/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 13:10:40 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/20 10:56:34 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H

# define CUB3D_H

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include "./libft/libft.h"
# include "minilibx-linux/mlx.h"

# define PI 3.1415926535
# define P2 PI/2
# define P3 3*PI/2
# define DR 0.0174533 // 1 degre = DR radians

typedef struct s_map
{
	char			*north;
	char			*south;
	char			*east;
	char			*west;
	unsigned char	floor[3];
	unsigned char	ceiling[3];
	char			**content;
	int				grid_height;
	int				grid_width;
	int				grid_start;
	char			**grid;
	int				p_x;
	int				p_y;
	char			p_dir;
}				t_map;

typedef struct s_data {
	void			*img;
	char			*addr;
	int				bpp;
	int				ll;
	int				endian;
}				t_data;

typedef struct s_vars {
	void			*mlx;
	void			*mlx_win;
}				t_vars;

typedef struct s_raystorage
{
	int		r;
	int		mx;
	int		my;
	int		mp; //useless dans un char **, utiliser mx/my
	int		dof;
	int		color; // temporary, useless
	float	rx;
	float	ry;
	float	ra;
	float	xo;
	float	yo;
	float	dis_h;
	float	hx;
	float	hy;
	float	dis_v;
	float	vx;
	float	vy;
	float	distance;
}				t_raystorage;

typedef struct s_game
{
	t_map			map;
	t_vars			*win;
	t_data			*img;
	float			player_x;
	float			player_y;
	float			player_deltax;
	float			player_deltay;
	float			player_angle;
	t_raystorage	*store;
}				t_game;

//minilibx_functions
void	init_mlx(t_game *game);
void	add_to_image(t_game *game);
void	draw_map(t_game *game);
void	draw_player(t_game *game);
void	draw_three_d(t_game *game, float distance, float ra);
void	pixel_put(t_data *data, int x, int y, int color);
float	dist(float ax, float ay, float bx, float by);

//raycasting
void	draw_ray(t_game *game);
void	distance_h(t_game *game, int pix);
void	distance_v(t_game *game, int pix);
void	loop_distance_v(t_game *game, int pix);
void	loop_distance_h(t_game *game, int pix);

//raycasting utils
void	pick_v_or_h(t_game *game, int pix);
void	init_draw_ray(t_game *game);
void	draw_minimap_dot(t_game *game);

//hooks
int		destroy_window(t_game *game);
void	close_program(t_game *game);
int		hook_slide(int keycode, t_game *game);
void	go_forward(t_game *game);
void	go_backward(t_game *game);
void	turn_right(t_game *game);
void	turn_left(t_game *game);

// parsing functions

void    free_tab(char **tab);
void    free_map(t_map *map);
void    parse_error(int n, t_map *map);
void	parse_cub_file(int ac, char *path, t_map *map);
void	set_rgb_values(t_map *map, char c, char *str, int i);
void	init_map_structure(t_map *map);
int     is_valid_extension(char *path);
void	extract_cub_file_content(char *path, t_map *map);
void	grid_char_check(char **c, t_map *map, int i, int j);
char	*get_texture_path(char **content, char *dir, int i, t_map *map);
void	closed_map_check(char **grid, t_map *map, int x, int y);
void	default_textures_paths_if_necessary(t_map *map);
void	check_if_directory(char *path, t_map *map, int fd);
int		skip_spaces(char *str, int i);
void    useless_lines_check(char **content, t_map *map, int i);
void	copy_map_grid(char **src, t_map *map, int i, int j);

#endif