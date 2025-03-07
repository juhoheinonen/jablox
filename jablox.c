#include "raylib.h"
#include "data_types.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "jablox.h"

// Global variables
game_status status = RUNNING;
const double move_horizontal_or_rotate_seconds = 0.05;
const double move_down_seconds = 0.3;
const int game_grid_width_in_tiles = 12; // 10 columns plus 2 walls
const int game_grid_height_in_tiles = 40;
int game_score = 0;
const int level_1_score_goal = 10;

int getRandomInt(int min, int max)
{
	return min + rand() % (max - min + 1);
}

tile_block_type getRandomBlockType()
{
	// int randomIndex = getRandomInt(I_BLOCK, Z_BLOCK);
	//  start with only giving some of the blocks
	int randomIndex = getRandomInt(I_BLOCK, O_BLOCK);
	return (tile_block_type)randomIndex;
}

occupied_xy_positions get_new_positions(block current_block);

void initialize_game_grid(game_tile game_grid[][40], int width, int height)
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			game_grid[x][y].x = x;
			game_grid[x][y].y = y;
			game_grid[x][y].type = EMPTY;
		}
	}

	// bottom horizontal wall
	for (int x = 0; x < width; x++)
	{
		game_grid[x][height - 1].type = WALL;
	}

	// vertical walls
	for (int y = 0; y < height; y++)
	{
		game_grid[0][y].type = WALL;
		game_grid[width - 1][y].type = WALL;
	}
}

void update_game_grid(game_tile game_grid[][40], block current_block)
{
	// clear previous occupied xy positions
	game_grid[current_block.previous_occupied_xy_positions.xy1.x][current_block.previous_occupied_xy_positions.xy1.y].type = EMPTY;
	game_grid[current_block.previous_occupied_xy_positions.xy2.x][current_block.previous_occupied_xy_positions.xy2.y].type = EMPTY;
	game_grid[current_block.previous_occupied_xy_positions.xy3.x][current_block.previous_occupied_xy_positions.xy3.y].type = EMPTY;
	game_grid[current_block.previous_occupied_xy_positions.xy4.x][current_block.previous_occupied_xy_positions.xy4.y].type = EMPTY;

	game_grid[current_block.occupied_xy_positions.xy1.x][current_block.occupied_xy_positions.xy1.y].type = current_block.block_type;
	game_grid[current_block.occupied_xy_positions.xy2.x][current_block.occupied_xy_positions.xy2.y].type = current_block.block_type;
	game_grid[current_block.occupied_xy_positions.xy3.x][current_block.occupied_xy_positions.xy3.y].type = current_block.block_type;
	game_grid[current_block.occupied_xy_positions.xy4.x][current_block.occupied_xy_positions.xy4.y].type = current_block.block_type;
}

void move_block_horizontal_or_rotate(game_tile game_grid[][40], block *current_block)
{
	// get all occupied xy positions
	occupied_xy_positions *positions = &current_block->occupied_xy_positions;
	occupied_xy_positions *previous_positions = &current_block->previous_occupied_xy_positions;

	switch (current_block->direction)
	{
	case DIRECTION_LEFT:
		// check if the block is not at the left wall
		if (positions->xy1.x > 1 && positions->xy2.x > 1 && positions->xy3.x > 1 && positions->xy4.x > 1)
		{
			// check if the block is not colliding with another block and allowing moving if the block is moving over own pieces
			if ((game_grid[positions->xy1.x - 1][positions->xy1.y].type == EMPTY || positions->xy1.x - 1 == positions->xy2.x || positions->xy1.x - 1 == positions->xy3.x || positions->xy1.x - 1 == positions->xy4.x) &&
				(game_grid[positions->xy2.x - 1][positions->xy2.y].type == EMPTY || positions->xy2.x - 1 == positions->xy1.x || positions->xy2.x - 1 == positions->xy3.x || positions->xy2.x - 1 == positions->xy4.x) &&
				(game_grid[positions->xy3.x - 1][positions->xy3.y].type == EMPTY || positions->xy3.x - 1 == positions->xy1.x || positions->xy3.x - 1 == positions->xy2.x || positions->xy3.x - 1 == positions->xy4.x) &&
				(game_grid[positions->xy4.x - 1][positions->xy4.y].type == EMPTY || positions->xy4.x - 1 == positions->xy1.x || positions->xy4.x - 1 == positions->xy2.x || positions->xy4.x - 1 == positions->xy3.x))
			{
				// store previous occupied xy positions
				previous_positions->xy1.x = positions->xy1.x;
				previous_positions->xy1.y = positions->xy1.y;
				previous_positions->xy2.x = positions->xy2.x;
				previous_positions->xy2.y = positions->xy2.y;
				previous_positions->xy3.x = positions->xy3.x;
				previous_positions->xy3.y = positions->xy3.y;
				previous_positions->xy4.x = positions->xy4.x;
				previous_positions->xy4.y = positions->xy4.y;

				// current_block->current_xy_position.x--;

				// move all occupied xy positions
				positions->xy1.x--;
				positions->xy2.x--;
				positions->xy3.x--;
				positions->xy4.x--;
			}
		}
		break;

	case DIRECTION_RIGHT:
		// check if the block is not at the right wall
		if (positions->xy1.x < 10 && positions->xy2.x < 10 && positions->xy3.x < 10 && positions->xy4.x < 10)
		{
			// check if the block is not colliding with another block
			if ((game_grid[positions->xy1.x + 1][positions->xy1.y].type == EMPTY || positions->xy1.x + 1 == positions->xy2.x || positions->xy1.x + 1 == positions->xy3.x || positions->xy1.x + 1 == positions->xy4.x) &&
				(game_grid[positions->xy2.x + 1][positions->xy2.y].type == EMPTY || positions->xy2.x + 1 == positions->xy1.x || positions->xy2.x + 1 == positions->xy3.x || positions->xy2.x + 1 == positions->xy4.x) &&
				(game_grid[positions->xy3.x + 1][positions->xy3.y].type == EMPTY || positions->xy3.x + 1 == positions->xy1.x || positions->xy3.x + 1 == positions->xy2.x || positions->xy3.x + 1 == positions->xy4.x) &&
				(game_grid[positions->xy4.x + 1][positions->xy4.y].type == EMPTY || positions->xy4.x + 1 == positions->xy1.x || positions->xy4.x + 1 == positions->xy2.x || positions->xy4.x + 1 == positions->xy3.x))
			{
				// store previous occupied xy positions
				previous_positions->xy1.x = positions->xy1.x;
				previous_positions->xy1.y = positions->xy1.y;
				previous_positions->xy2.x = positions->xy2.x;
				previous_positions->xy2.y = positions->xy2.y;
				previous_positions->xy3.x = positions->xy3.x;
				previous_positions->xy3.y = positions->xy3.y;
				previous_positions->xy4.x = positions->xy4.x;
				previous_positions->xy4.y = positions->xy4.y;

				// current_block->current_xy_position.x++;

				// move all occupied xy positions
				positions->xy1.x++;
				positions->xy2.x++;
				positions->xy3.x++;
				positions->xy4.x++;
			}
		}
		break;

	case DIRECTION_ROTATE:
		// rotate_block(game_grid, current_block);
		// xy_position *position = &current_block->current_xy_position;

		// store previous occupied xy positions
		previous_positions->xy1.x = positions->xy1.x;
		previous_positions->xy1.y = positions->xy1.y;
		previous_positions->xy2.x = positions->xy2.x;
		previous_positions->xy2.y = positions->xy2.y;
		previous_positions->xy3.x = positions->xy3.x;
		previous_positions->xy3.y = positions->xy3.y;
		previous_positions->xy4.x = positions->xy4.x;
		previous_positions->xy4.y = positions->xy4.y;

		xy_position position = {positions->xy1.x, positions->xy1.y};

		// check current rotation and calculate next position based on it. TODO: generalize this to other blocks
		if (current_block->block_type == I_BLOCK)
		{
			if (current_block->rotation == RIGHT)
			{
				// turn down
				positions->xy1.x = position.x;
				positions->xy1.y = position.y;
				positions->xy2.x = position.x;
				positions->xy2.y = position.y + 1;
				positions->xy3.x = position.x;
				positions->xy3.y = position.y + 2;
				positions->xy4.x = position.x;
				positions->xy4.y = position.y + 3;
				current_block->rotation = DOWN;
			}
			else if (current_block->rotation == DOWN)
			{
				// turn left
				positions->xy1.x = position.x;
				positions->xy1.y = position.y;
				positions->xy2.x = position.x - 1;
				positions->xy2.y = position.y;
				positions->xy3.x = position.x - 2;
				positions->xy3.y = position.y;
				positions->xy4.x = position.x - 3;
				positions->xy4.y = position.y;
				current_block->rotation = LEFT;
			}
			else if (current_block->rotation == LEFT)
			{
				// turn up
				positions->xy1.x = position.x;
				positions->xy1.y = position.y;
				positions->xy2.x = position.x;
				positions->xy2.y = position.y - 1;
				positions->xy3.x = position.x;
				positions->xy3.y = position.y - 2;
				positions->xy4.x = position.x;
				positions->xy4.y = position.y - 3;
				current_block->rotation = UP;
			}
			else if (current_block->rotation == UP)
			{
				// turn right
				positions->xy1.x = position.x;
				positions->xy1.y = position.y;
				positions->xy2.x = position.x + 1;
				positions->xy2.y = position.y;
				positions->xy3.x = position.x + 2;
				positions->xy3.y = position.y;
				positions->xy4.x = position.x + 3;
				positions->xy4.y = position.y;
				current_block->rotation = RIGHT;
			}
		}
		else if (current_block->block_type == J_BLOCK)
		{
			if (current_block->rotation == RIGHT)
			{
				positions->xy1.x++;
				positions->xy2.y--;
				positions->xy3.x--;
				positions->xy4.x--;
				positions->xy4.x--;
				positions->xy4.y++;
				current_block->rotation = DOWN;
			}
			else if (current_block->rotation == DOWN)
			{
				positions->xy1.y++;
				positions->xy2.x++;
				positions->xy3.y--;
				positions->xy4.x--;
				positions->xy4.y--;
				positions->xy4.y--;
				current_block->rotation = LEFT;
			}
			else if (current_block->rotation == LEFT)
			{
				positions->xy3.y++;
				positions->xy4.y--;
				positions->xy4.x++;
				positions->xy4.x++;
				current_block->rotation = UP;
			}
			else if (current_block->rotation == UP)
			{
				positions->xy1.x--;
				positions->xy1.y--;
				positions->xy2.x--;
				positions->xy2.y++;
				positions->xy3.x++;
				positions->xy4.x++;
				positions->xy4.y++;
				positions->xy4.y++;
				current_block->rotation = RIGHT;
			}
		}
		else if (current_block->block_type == L_BLOCK)
		{
			occupied_xy_positions new_positions = get_new_positions(*current_block);
			positions->xy1.x = new_positions.xy1.x;
			positions->xy1.y = new_positions.xy1.y;
			positions->xy2.x = new_positions.xy2.x;
			positions->xy2.y = new_positions.xy2.y;
			positions->xy3.x = new_positions.xy3.x;
			positions->xy3.y = new_positions.xy3.y;
			positions->xy4.x = new_positions.xy4.x;
			positions->xy4.y = new_positions.xy4.y;
			if (current_block->rotation == RIGHT)
			{
				current_block->rotation = DOWN;
			}
			else if (current_block->rotation == DOWN)
			{
				current_block->rotation = LEFT;
			}
			else if (current_block->rotation == LEFT)
			{
				current_block->rotation = UP;
			}
			else if (current_block->rotation == UP)
			{
				current_block->rotation = RIGHT;
			}
		}
		break;
	}
	current_block->direction = DIRECTION_DOWN;
}

void move_block_down(game_tile game_grid[][40], block *current_block)
{
	// get all occupied xy positions
	occupied_xy_positions *positions = &current_block->occupied_xy_positions;
	occupied_xy_positions *previous_positions = &current_block->previous_occupied_xy_positions;

	// check if the block is not at the bottom wall
	if (positions->xy1.y < 39 && positions->xy2.y < 39 && positions->xy3.y < 39 && positions->xy4.y < 39)
	{
		// check if the block is not colliding with another block
		if ((game_grid[positions->xy1.x][positions->xy1.y + 1].type == EMPTY || positions->xy1.y + 1 == positions->xy2.y || positions->xy1.y + 1 == positions->xy3.y || positions->xy1.y + 1 == positions->xy4.y) &&
			(game_grid[positions->xy2.x][positions->xy2.y + 1].type == EMPTY || positions->xy2.y + 1 == positions->xy1.y || positions->xy2.y + 1 == positions->xy3.y || positions->xy2.y + 1 == positions->xy4.y) &&
			(game_grid[positions->xy3.x][positions->xy3.y + 1].type == EMPTY || positions->xy3.y + 1 == positions->xy1.y || positions->xy3.y + 1 == positions->xy2.y || positions->xy3.y + 1 == positions->xy4.y) &&
			(game_grid[positions->xy4.x][positions->xy4.y + 1].type == EMPTY || positions->xy4.y + 1 == positions->xy1.y || positions->xy4.y + 1 == positions->xy2.y || positions->xy4.y + 1 == positions->xy3.y))
		{
			// store previous occupied xy positions
			previous_positions->xy1.x = positions->xy1.x;
			previous_positions->xy1.y = positions->xy1.y;
			previous_positions->xy2.x = positions->xy2.x;
			previous_positions->xy2.y = positions->xy2.y;
			previous_positions->xy3.x = positions->xy3.x;
			previous_positions->xy3.y = positions->xy3.y;
			previous_positions->xy4.x = positions->xy4.x;
			previous_positions->xy4.y = positions->xy4.y;

			// current_block->current_xy_position.y++;

			// move all occupied xy positions
			positions->xy1.y++;
			positions->xy2.y++;
			positions->xy3.y++;
			positions->xy4.y++;
		}
	}

	current_block->direction = DIRECTION_DOWN;
}

int check_hit_down(game_tile game_grid[][40], block current_block)
{
	// get all occupied xy positions
	occupied_xy_positions *positions = &current_block.occupied_xy_positions;

	// check if the block is not at the bottom wall
	if (positions->xy1.y < 39 && positions->xy2.y < 39 && positions->xy3.y < 39 && positions->xy4.y < 39)
	{
		// check if the block is not colliding with another block
		if ((game_grid[positions->xy1.x][positions->xy1.y + 1].type == EMPTY || positions->xy1.y + 1 == positions->xy2.y || positions->xy1.y + 1 == positions->xy3.y || positions->xy1.y + 1 == positions->xy4.y) &&
			(game_grid[positions->xy2.x][positions->xy2.y + 1].type == EMPTY || positions->xy2.y + 1 == positions->xy1.y || positions->xy2.y + 1 == positions->xy3.y || positions->xy2.y + 1 == positions->xy4.y) &&
			(game_grid[positions->xy3.x][positions->xy3.y + 1].type == EMPTY || positions->xy3.y + 1 == positions->xy1.y || positions->xy3.y + 1 == positions->xy2.y || positions->xy3.y + 1 == positions->xy4.y) &&
			(game_grid[positions->xy4.x][positions->xy4.y + 1].type == EMPTY || positions->xy4.y + 1 == positions->xy1.y || positions->xy4.y + 1 == positions->xy2.y || positions->xy4.y + 1 == positions->xy3.y))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
}

void mark_block_as_landed(block *current_block)
{
	switch (current_block->block_type)
	{
	case I_BLOCK:
		current_block->block_type = I_BLOCK_FALLEN;
		break;
	case J_BLOCK:
		current_block->block_type = J_BLOCK_FALLEN;
		break;
	case L_BLOCK:
		current_block->block_type = L_BLOCK_FALLEN;
		break;
	case O_BLOCK:
		current_block->block_type = O_BLOCK_FALLEN;
		break;
	case S_BLOCK:
		current_block->block_type = S_BLOCK_FALLEN;
		break;
	case T_BLOCK:
		current_block->block_type = T_BLOCK_FALLEN;
		break;
	case Z_BLOCK:
		current_block->block_type = Z_BLOCK_FALLEN;
		break;
	}
}

occupied_xy_positions create_occupied_xy_positions(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	occupied_xy_positions positions;
	positions.xy1.x = x1;
	positions.xy1.y = y1;
	positions.xy2.x = x2;
	positions.xy2.y = y2;
	positions.xy3.x = x3;
	positions.xy3.y = y3;
	positions.xy4.x = x4;
	positions.xy4.y = y4;
	return positions;
}

occupied_xy_positions get_initial_xy_positions_by_block_type(tile_block_type block_type)
{
	switch (block_type)
	{
	case I_BLOCK:
		return create_occupied_xy_positions(2, 20, 3, 20, 4, 20, 5, 20);
		break;
	case J_BLOCK:
		return create_occupied_xy_positions(2, 19, 2, 20, 3, 20, 4, 20);
		break;
	case L_BLOCK:
		return create_occupied_xy_positions(2, 20, 3, 20, 4, 20, 4, 19);
		break;
	case O_BLOCK:
		return create_occupied_xy_positions(2, 20, 3, 20, 2, 19, 3, 19);
		break;
		// Add other cases for different block types when needed
	}
}

block initialize_new_block()
{
	tile_block_type current_block_type = getRandomBlockType();

	occupied_xy_positions initial_occupied_xy_positions = get_initial_xy_positions_by_block_type(current_block_type);

	occupied_xy_positions previous_occupied_positions = initial_occupied_xy_positions;

	drop_speed initial_drop_speed = NORMAL;
	block current_block = {current_block_type, RIGHT, DIRECTION_DOWN, initial_occupied_xy_positions, previous_occupied_positions, initial_drop_speed};

	return current_block;
}

occupied_xy_positions get_new_positions(block current_block)
{
	occupied_xy_positions positions = current_block.occupied_xy_positions;

	occupied_xy_positions new_positions;
	switch (current_block.block_type)
	{
	case L_BLOCK:

		// return create_occupied_xy_positions(2, 20, 3, 20, 4, 20, 4, 19);

		if (current_block.rotation == RIGHT)
		{
			new_positions.xy1.x = positions.xy1.x;
			new_positions.xy1.y = positions.xy1.y - 2;
			new_positions.xy2.x = positions.xy2.x - 1;
			new_positions.xy2.y = positions.xy2.y - 1;
			new_positions.xy3.x = positions.xy3.x - 2;
			new_positions.xy3.y = positions.xy3.y;
			new_positions.xy4.x = positions.xy4.x - 1;
			new_positions.xy4.y = positions.xy4.y + 1;
		}
		else if (current_block.rotation == DOWN)
		{
			new_positions.xy1.x = positions.xy1.x + 2;
			new_positions.xy1.y = positions.xy1.y;
			new_positions.xy2.x = positions.xy2.x + 1;
			new_positions.xy2.y = positions.xy2.y - 1;
			new_positions.xy3.x = positions.xy3.x;
			new_positions.xy3.y = positions.xy3.y - 2;
			new_positions.xy4.x = positions.xy4.x - 1;
			new_positions.xy4.y = positions.xy4.y - 1;
		}
		else if (current_block.rotation == LEFT)
		{
			new_positions.xy1.x = positions.xy1.x - 1;
			new_positions.xy1.y = positions.xy1.y + 2;
			new_positions.xy2.x = positions.xy2.x;
			new_positions.xy2.y = positions.xy2.y + 1;
			new_positions.xy3.x = positions.xy3.x + 1;
			new_positions.xy3.y = positions.xy3.y;
			new_positions.xy4.x = positions.xy4.x;
			new_positions.xy4.y = positions.xy4.y - 1;
		}
		else if (current_block.rotation == UP)
		{
			new_positions.xy1.x = positions.xy1.x - 1;
			new_positions.xy1.y = positions.xy1.y - 1;
			new_positions.xy2.x = positions.xy2.x;
			new_positions.xy2.y = positions.xy2.y;
			new_positions.xy3.x = positions.xy3.x + 1;
			new_positions.xy3.y = positions.xy3.y + 1;
			new_positions.xy4.x = positions.xy4.x + 2;
			new_positions.xy4.y = positions.xy4.y;
		}

		break;

	case O_BLOCK:
		// this block does not need rotating
		new_positions = positions;
		break;
	}

	return new_positions;
}

int can_rotate(game_tile game_grid[][40], block current_block)
{
	occupied_xy_positions *positions = &current_block.occupied_xy_positions;
	occupied_xy_positions new_positions;
	rotation current_rotation = current_block.rotation;

	switch (current_block.block_type)
	{
	case I_BLOCK:
		if (current_rotation == RIGHT)
		{
			// check turn down
			new_positions.xy1.x = positions->xy1.x;
			new_positions.xy1.y = positions->xy1.y;
			new_positions.xy2.x = positions->xy1.x;
			new_positions.xy2.y = positions->xy1.y + 1;
			new_positions.xy3.x = positions->xy1.x;
			new_positions.xy3.y = positions->xy1.y + 2;
			new_positions.xy4.x = positions->xy1.x;
			new_positions.xy4.y = positions->xy1.y + 3;
		}
		else if (current_rotation == DOWN)
		{
			// check turn left
			new_positions.xy1.x = positions->xy1.x;
			new_positions.xy1.y = positions->xy1.y;
			new_positions.xy2.x = positions->xy1.x - 1;
			new_positions.xy2.y = positions->xy1.y;
			new_positions.xy3.x = positions->xy1.x - 2;
			new_positions.xy3.y = positions->xy1.y;
			new_positions.xy4.x = positions->xy1.x - 3;
			new_positions.xy4.y = positions->xy1.y;
		}
		else if (current_rotation == LEFT)
		{
			// check turn up
			new_positions.xy1.x = positions->xy1.x;
			new_positions.xy1.y = positions->xy1.y;
			new_positions.xy2.x = positions->xy1.x;
			new_positions.xy2.y = positions->xy1.y - 1;
			new_positions.xy3.x = positions->xy1.x;
			new_positions.xy3.y = positions->xy1.y - 2;
			new_positions.xy4.x = positions->xy1.x;
			new_positions.xy4.y = positions->xy1.y - 3;
		}
		else if (current_rotation == UP)
		{
			// check turn right
			new_positions.xy1.x = positions->xy1.x;
			new_positions.xy1.y = positions->xy1.y;
			new_positions.xy2.x = positions->xy1.x + 1;
			new_positions.xy2.y = positions->xy1.y;
			new_positions.xy3.x = positions->xy1.x + 2;
			new_positions.xy3.y = positions->xy1.y;
			new_positions.xy4.x = positions->xy1.x + 3;
			new_positions.xy4.y = positions->xy1.y;
		}
		break;
	case J_BLOCK:
		if (current_rotation == RIGHT)
		{
			new_positions.xy1.x = positions->xy1.x + 1;
			new_positions.xy1.y = positions->xy1.y;
			new_positions.xy2.x = positions->xy2.x;
			new_positions.xy2.y = positions->xy2.y - 1;
			new_positions.xy3.x = positions->xy3.x - 1;
			new_positions.xy3.y = positions->xy3.y;
			new_positions.xy4.x = positions->xy4.x - 2;
			new_positions.xy4.y = positions->xy4.y + 1;
		}
		else if (current_rotation == DOWN)
		{
			new_positions.xy1.x = positions->xy1.x;
			new_positions.xy1.y = positions->xy1.y + 1;
			new_positions.xy2.x = positions->xy2.x + 1;
			new_positions.xy2.y = positions->xy2.y;
			new_positions.xy3.x = positions->xy3.x;
			new_positions.xy3.y = positions->xy3.y - 1;
			new_positions.xy4.x = positions->xy4.x - 1;
			new_positions.xy4.y = positions->xy4.y - 2;
		}
		else if (current_rotation == LEFT)
		{
			new_positions.xy1.x = positions->xy1.x;
			new_positions.xy1.y = positions->xy1.y;
			new_positions.xy2.x = positions->xy2.x;
			new_positions.xy2.y = positions->xy2.y + 1;
			new_positions.xy3.x = positions->xy3.x;
			new_positions.xy3.y = positions->xy3.y + 2;
			new_positions.xy4.x = positions->xy4.x + 1;
			new_positions.xy4.y = positions->xy4.y - 1;
		}
		else if (current_rotation == UP)
		{
			new_positions.xy1.x = positions->xy1.x - 1;
			new_positions.xy1.y = positions->xy1.y - 1;
			new_positions.xy2.x = positions->xy2.x - 1;
			new_positions.xy2.y = positions->xy2.y + 1;
			new_positions.xy3.x = positions->xy3.x + 1;
			new_positions.xy3.y = positions->xy3.y;
			new_positions.xy4.x = positions->xy4.x + 2;
			new_positions.xy4.y = positions->xy4.y + 1;
		}
		break;
	case L_BLOCK:
		new_positions = get_new_positions(current_block);
		break;
	default:
		return 0;
	}

	// Check if the new positions are valid (not colliding with walls or other blocks)
	if (new_positions.xy1.x > 0 && new_positions.xy1.x < game_grid_width_in_tiles - 1 &&
		new_positions.xy2.x > 0 && new_positions.xy2.x < game_grid_width_in_tiles - 1 &&
		new_positions.xy3.x > 0 && new_positions.xy3.x < game_grid_width_in_tiles - 1 &&
		new_positions.xy4.x > 0 && new_positions.xy4.x < game_grid_width_in_tiles - 1 &&
		(game_grid[new_positions.xy1.x][new_positions.xy1.y].type == EMPTY || game_grid[new_positions.xy1.x][new_positions.xy1.y].type == current_block.block_type) &&
		(game_grid[new_positions.xy2.x][new_positions.xy2.y].type == EMPTY || game_grid[new_positions.xy2.x][new_positions.xy2.y].type == current_block.block_type) &&
		(game_grid[new_positions.xy3.x][new_positions.xy3.y].type == EMPTY || game_grid[new_positions.xy3.x][new_positions.xy3.y].type == current_block.block_type) &&
		(game_grid[new_positions.xy4.x][new_positions.xy4.y].type == EMPTY || game_grid[new_positions.xy4.x][new_positions.xy4.y].type == current_block.block_type))
	{
		return 1;
	}

	return 0;
}

int mark_whole_rows(game_tile game_grid[][40])
{
	int whole_rows = 0;

	// todo only do this until starting position. Skip walls
	for (size_t y = game_grid_height_in_tiles - 2; y > 0; y--)
	{
		int whole_row_found = 0;
		for (size_t x = game_grid_width_in_tiles - 2; x > 0; x--)
		{
			game_tile tile = game_grid[x][y];
			if (tile.type < I_BLOCK_FALLEN || tile.type > Z_BLOCK_FALLEN)
			{
				// not full row
				break;
			}
			if (x == 1)
			{
				whole_row_found = 1;
				whole_rows++;
				// clear the row
				for (size_t x1 = 1; x1 < game_grid_width_in_tiles - 1; x1++)
				{
					game_grid[x1][y].type = EMPTY;
				}
				// move other rows down
				for (size_t y1 = y - 1; y1 > 0; y1--)
				{
					for (size_t x2 = game_grid_width_in_tiles - 2; x2 > 0; x2--)
					{
						tile = game_grid[x2][y1];
						// assign the tile's type to lower row
						game_grid[x2][y1 + 1].type = tile.type;
					}
				}
			}
		}
	}

	// recursively call this function again to get number of full rows;
	if (whole_rows > 0)
	{
		whole_rows += mark_whole_rows(game_grid);
	}

	return whole_rows;
}

void draw_grid(const int game_grid_width_in_tiles, const int game_grid_height_in_tiles, game_tile game_grid[][40], const int horizontal_offset, const int tile_width, const int vertical_offset, const int tile_height)
{
	for (int x = 0; x < game_grid_width_in_tiles; x++)
	{
		for (int y = 0; y < game_grid_height_in_tiles; y++)
		{
			switch (game_grid[x][y].type)
			{
			case EMPTY:
				DrawRectangle(horizontal_offset + x * tile_width, vertical_offset + y * tile_height, tile_width, tile_height, GRAY);
				break;
			case WALL:
				DrawRectangle(horizontal_offset + x * tile_width, vertical_offset + y * tile_height, tile_width, tile_height, BROWN);
				break;
			case I_BLOCK:
			case I_BLOCK_FALLEN:
				// light blue
				DrawRectangle(horizontal_offset + x * tile_width, vertical_offset + y * tile_height, tile_width, tile_height, (Color){0, 121, 241, 200});
				break;
			case J_BLOCK:
			case J_BLOCK_FALLEN:
				// blue
				DrawRectangle(horizontal_offset + x * tile_width, vertical_offset + y * tile_height, tile_width, tile_height, (Color){0, 121, 241, 255});
				break;
			case L_BLOCK:
			case L_BLOCK_FALLEN:
				DrawRectangle(horizontal_offset + x * tile_width, vertical_offset + y * tile_height, tile_width, tile_height, ORANGE);
				break;
			case O_BLOCK:
			case O_BLOCK_FALLEN:
				DrawRectangle(horizontal_offset + x * tile_width, vertical_offset + y * tile_height, tile_width, tile_height, YELLOW);
				break;
			case S_BLOCK:
			case S_BLOCK_FALLEN:
				DrawRectangle(horizontal_offset + x * tile_width, vertical_offset + y * tile_height, tile_width, tile_height, GREEN);
				break;
			case T_BLOCK:
			case T_BLOCK_FALLEN:
				DrawRectangle(horizontal_offset + x * tile_width, vertical_offset + y * tile_height, tile_width, tile_height, PURPLE);
				break;
			case Z_BLOCK:
			case Z_BLOCK_FALLEN:
				DrawRectangle(horizontal_offset + x * tile_width, vertical_offset + y * tile_height, tile_width, tile_height, RED);
				break;
			}
			// draw grid lines
			DrawRectangleLines(horizontal_offset + x * tile_width, vertical_offset + y * tile_height, tile_width, tile_height, BLACK);
		}
	}
}

int main(void)
{
	const int screenWidth = 2048;	  // 1024;
	const int screenHeight = 768 * 3; // 1536; // 768;

	const int horizontal_offset = (screenWidth / 2) / 2;
	const int vertical_offset = 150;

	const int tile_width = 48;	// screenWidth / game_grid_width_in_tiles;
	const int tile_height = 48; // screenHeight / game_grid_height_in_tiles;

	InitWindow(screenWidth, screenHeight, "Jablox");

	// Declare a fixed-size 2D array
	game_tile game_grid[game_grid_width_in_tiles][game_grid_height_in_tiles];

	initialize_game_grid(game_grid, game_grid_width_in_tiles, game_grid_height_in_tiles);

	double seconds_elapsed_for_move_horizontal_or_rotate = 0.0;
	double seconds_elapsed_for_move_down = 0.0;

	srand(time(NULL)); // Initialize random seed

	block current_block = initialize_new_block();

	update_game_grid(game_grid, current_block);

	while (!WindowShouldClose())
	{
		if (status == WIN)
		{
			while (!IsKeyDown(KEY_ENTER) && !IsKeyDown(KEY_ESCAPE) && !WindowShouldClose())
			{
				BeginDrawing();
				ClearBackground(RAYWHITE);
				DrawText(TextFormat("Yes, you finished Jablox!!!"), screenWidth / 2, 400, 50, BLACK);
				EndDrawing();
			}
			break;
		}
		else if (status == GAME_OVER)
		{
			while (!IsKeyDown(KEY_ENTER) && !IsKeyDown(KEY_ESCAPE) && !WindowShouldClose())
			{
				BeginDrawing();
				ClearBackground(RAYWHITE);
				DrawText(TextFormat("Game over. No worries, it's only a game!!!"), screenWidth / 2, 400, 50, BLACK);
				EndDrawing();
			}
			break;
		}

		// Listen for direction keys
		if (IsKeyPressed(KEY_LEFT))
		{
			current_block.direction = DIRECTION_LEFT;
			// TraceLog(LOG_INFO, "Left");
		}
		if (IsKeyPressed(KEY_RIGHT))
		{
			current_block.direction = DIRECTION_RIGHT;
		}
		if (IsKeyPressed(KEY_DOWN))
		{
			current_block.direction = DIRECTION_DOWN;
			current_block.drop_speed = FAST;
		}
		if (IsKeyPressed(KEY_SPACE))
		{
			current_block.direction = DIRECTION_DOWN;
			current_block.drop_speed = STRAIGHT_DOWN;
		}

		// if block also for rotation
		if (IsKeyPressed(KEY_UP))
		{
			if (can_rotate(game_grid, current_block))
			{
				current_block.direction = DIRECTION_ROTATE; // rotate_block(game_grid, &current_block);
			}
		}

		// Update the game state based on the elapsed time
		seconds_elapsed_for_move_down += GetFrameTime();
		// TraceLog(LOG_INFO, "Seconds elapsed for move down: %f", seconds_elapsed_for_move_down);
		seconds_elapsed_for_move_horizontal_or_rotate += seconds_elapsed_for_move_down;

		if (seconds_elapsed_for_move_horizontal_or_rotate > move_horizontal_or_rotate_seconds)
		{
			seconds_elapsed_for_move_horizontal_or_rotate = 0.0;
			if (current_block.direction == DIRECTION_LEFT || current_block.direction == DIRECTION_RIGHT || current_block.direction == DIRECTION_ROTATE)
			{
				move_block_horizontal_or_rotate(game_grid, &current_block);
				update_game_grid(game_grid, current_block);
			}
		}
		if (seconds_elapsed_for_move_down > move_down_seconds || current_block.drop_speed == FAST || current_block.drop_speed == STRAIGHT_DOWN)
		{
			seconds_elapsed_for_move_down = 0.0;

			int hit_down = 0;

			if (current_block.drop_speed == STRAIGHT_DOWN)
			{
				while (hit_down == 0)
				{
					move_block_down(game_grid, &current_block);
					hit_down = check_hit_down(game_grid, current_block);
					if (hit_down)
					{
						mark_block_as_landed(&current_block);
					}
					update_game_grid(game_grid, current_block);
				}
				current_block = initialize_new_block();
			}
			else
			{
				move_block_down(game_grid, &current_block);

				hit_down = check_hit_down(game_grid, current_block);
				if (hit_down)
				{
					mark_block_as_landed(&current_block);
					update_game_grid(game_grid, current_block);
					current_block = initialize_new_block();
				}
				else
				{
					current_block.drop_speed = NORMAL;
					update_game_grid(game_grid, current_block);
				}
			}

			if (hit_down)
			{
				game_score += mark_whole_rows(game_grid);

				if (game_score >= level_1_score_goal)
				{
					status = WIN;
				}
			}
		}

		BeginDrawing();

		ClearBackground(RAYWHITE);

		// Draw the game grid. Currently just use colors to fill the tiles. Empty is light green, wall is brown.
		draw_grid(game_grid_width_in_tiles, game_grid_height_in_tiles, game_grid, horizontal_offset, tile_width, vertical_offset, tile_height);
		// write score on right side of the game area
		DrawText(TextFormat("Score: %d", game_score), screenWidth - 800, 400, 50, BLACK);

		EndDrawing();
	}

	return 0;
}
