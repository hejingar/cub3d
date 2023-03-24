/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 12:03:34 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/21 16:02:30 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	main(int ac, char **av)
{
	t_game	game;

	parse_cub_file(ac, av[1], &game.map);
	game.player_y = 512 / game.map.grid_height * game.map.p_y;
	game.player_x = 512 / game.map.grid_height * game.map.p_x;
	game.player_angle = 0.1;
	game.player_deltax = cos(game.player_angle) * 5;
	game.player_deltay = sin(game.player_angle) * 5;
	init_mlx(&game);
	return (0);
}
