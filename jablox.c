#include "raylib.h"
#include "data_types.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "jablox.h"

// Global variables
game_status status = RUNNING;
const double move_horizontal_or_rotate_seconds = 0.1;
const double move_down_seconds = 0.5;

int getRandomInt(int min, int max)
{
	return (rand() % max) + min;
}

tile_block_type getRandomBlockType()
{
	int randomIndex = getRandomInt(I_BLOCK, Z_BLOCK);
	return (tile_block_type)randomIndex;
}

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

void update_block_to_grid(game_tile game_grid[][40], block current_block)
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

				current_block->current_xy_position.x--;

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

				current_block->current_xy_position.x++;

				// move all occupied xy positions
				positions->xy1.x++;
				positions->xy2.x++;
				positions->xy3.x++;
				positions->xy4.x++;
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

			current_block->current_xy_position.y++;

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

// todo: thing about initial position etc.
block initialize_new_block()
{
	// tile_block_type current_block_type = getRandomBlockType();
	//  initially hard-coded to I_BLOCK for development
	tile_block_type current_block_type = I_BLOCK;
	occupied_xy_positions occupied_positions = {{2, 20}, {3, 20}, {4, 20}, {5, 20}};
	occupied_xy_positions previous_occupied_positions = {{2, 20}, {3, 20}, {4, 20}, {5, 20}};

	int fast_drop = 0;

	block current_block = {{2, 20}, current_block_type, RIGHT, DIRECTION_DOWN, occupied_positions, previous_occupied_positions, fast_drop};

	return current_block;
}

int can_rotate(game_tile game_grid[][40], block current_block) {
	// get all occupied xy positions
	occupied_xy_positions *positions = &current_block.occupied_xy_positions;

	rotation current_rotation = current_block.rotation;
	rotation new_rotation = (current_rotation + 1) % 4;

	// // check if the block is not at the bottom wall
	// if (positions->xy1.y < 39 && positions->xy2.y < 39 && positions->xy3.y < 39 && positions->xy4.y < 39)
	// {
	// 	// check if the block is not colliding with another block
	// 	if ((game_grid[positions->xy1.x][positions->xy1.y + 1].type == EMPTY || positions->xy1.y + 1 == positions->xy2.y || positions->xy1.y + 1 == positions->xy3.y || positions->xy1.y + 1 == positions->xy4.y) &&
	// 		(game_grid[positions->xy2.x][positions->xy2.y + 1].type == EMPTY || positions->xy2.y + 1 == positions->xy1.y || positions->xy2.y + 1 == positions->xy3.y || positions->xy2.y + 1 == positions->xy4.y) &&
	// 		(game_grid[positions->xy3.x][positions->xy3.y + 1].type == EMPTY || positions->xy3.y + 1 == positions->xy1.y || positions->xy3.y + 1 == positions->xy2.y || positions->xy3.y + 1 == positions->xy4.y) &&
	// 		(game_grid[positions->xy4.x][positions->xy4.y + 1].type == EMPTY || positions->xy4.y + 1 == positions->xy1.y || positions->xy4.y + 1 == positions->xy2.y || positions->xy4.y + 1 == positions->xy3.y))
	// 	{
	// 		return 1;
	// 	}
	// 	else
	// 	{
	// 		return 0;
	// 	}
	// }
	// else
	// {
	// 	return 0;
	// }
}

void draw_grid(const int game_grid_width_in_tiles, const int game_grid_height_in_tiles, game_tile game_grid[12][40], const int horizontal_offset, const int tile_width, const int vertical_offset, const int tile_height)
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
				DrawRectangle(horizontal_offset + x * tile_width, vertical_offset + y * tile_height, tile_width, tile_height, BLUE);
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

	const int game_grid_width_in_tiles = 12; // 10 columns plus 2 walls
	const int game_grid_height_in_tiles = 40;

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

	// tile_block_type current_block_type = getRandomBlockType();
	//  initially hard-coded to I_BLOCK for development
	// tile_block_type current_block_type = I_BLOCK;
	// occupied_xy_positions occupied_positions = {{2, 20}, {3, 20}, {4, 20}, {5, 20}};
	// occupied_xy_positions previous_occupied_positions = {{2, 20}, {3, 20}, {4, 20}, {5, 20}};

	// block current_block = {{2, 20}, current_block_type, RIGHT, DIRECTION_DOWN, occupied_positions, previous_occupied_positions};

	block current_block = initialize_new_block();

	update_block_to_grid(game_grid, current_block);

	while (!WindowShouldClose())
	{
		BeginDrawing();

		// Listen for direction keys
		if (IsKeyPressed(KEY_LEFT))
		{
			current_block.direction = DIRECTION_LEFT;
			TraceLog(LOG_INFO, "Left");
		}
		if (IsKeyPressed(KEY_RIGHT))
		{
			current_block.direction = DIRECTION_RIGHT;
		}
		if (IsKeyPressed(KEY_DOWN))
		{
			current_block.direction = DIRECTION_DOWN;
			current_block.fast_drop = 1;
		}
		// if block also for rotation
		if (IsKeyPressed(KEY_UP))
		{
			if (can_rotate(game_grid, current_block))
			{
				rotate_block(game_grid, &current_block);
			}
		}

		// Update the game state based on the elapsed time
		seconds_elapsed_for_move_down += GetFrameTime();
		// TraceLog(LOG_INFO, "Seconds elapsed for move down: %f", seconds_elapsed_for_move_down);
		seconds_elapsed_for_move_horizontal_or_rotate += seconds_elapsed_for_move_down;

		if (seconds_elapsed_for_move_horizontal_or_rotate > move_horizontal_or_rotate_seconds)
		{
			seconds_elapsed_for_move_horizontal_or_rotate = 0.0;
			if (current_block.direction == DIRECTION_LEFT || current_block.direction == DIRECTION_RIGHT)
			{
				move_block_horizontal_or_rotate(game_grid, &current_block);
				update_block_to_grid(game_grid, current_block);
			}
		}
		if (seconds_elapsed_for_move_down > move_down_seconds || current_block.fast_drop)
		{
			seconds_elapsed_for_move_down = 0.0;
			current_block.fast_drop = 0;
			move_block_down(game_grid, &current_block);

			int hit_down = check_hit_down(game_grid, current_block);
			if (hit_down)
			{
				mark_block_as_landed(&current_block);
				update_block_to_grid(game_grid, current_block);
				current_block = initialize_new_block();
			}
			else
			{
				update_block_to_grid(game_grid, current_block);
			}

			// todo, many things:
			// --> check if block has landed
			// --> check if game over
			// --> check if row is full
		}

		ClearBackground(RAYWHITE);

		// Draw the game grid. Currently just use colors to fill the tiles. Empty is light green, wall is brown.
		draw_grid(game_grid_width_in_tiles, game_grid_height_in_tiles, game_grid, horizontal_offset, tile_width, vertical_offset, tile_height);

		EndDrawing();
	}

	return 0;
}
