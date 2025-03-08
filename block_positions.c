#include "block_positions.h"
#include "data_types.h"

occupied_xy_positions create_occupied_xy_positions(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int x6, int y6)
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
	positions.xy5.x = x5;
	positions.xy5.y = y5;
	positions.xy6.x = x6;
	positions.xy6.y = y6;
	return positions;
}

occupied_xy_positions get_initial_xy_positions_by_block_type(tile_block_type block_type)
{
	int initial_y = 10;

	switch (block_type)
	{
	case I_BLOCK:
		return create_occupied_xy_positions(2, initial_y, 3, initial_y, 4, initial_y, 5, initial_y, 0, 0, 0, 0);
		break;
	case J_BLOCK:
		return create_occupied_xy_positions(2, initial_y - 1, 2, initial_y, 3, initial_y, 4, initial_y, 0, 0, 0, 0);
		break;
	case L_BLOCK:
		return create_occupied_xy_positions(2, initial_y, 3, initial_y, 4, initial_y, 4, initial_y - 1, 0, 0, 0, 0);
		break;
	case O_BLOCK:
		return create_occupied_xy_positions(2, initial_y, 3, initial_y, 2, initial_y - 1, 3, initial_y - 1, 0, 0, 0, 0);
		break;
	case S_BLOCK:
		return create_occupied_xy_positions(2, initial_y, 3, initial_y, 3, initial_y - 1, 4, initial_y - 1, 0, 0, 0, 0);
		break;
	case T_BLOCK:
		return create_occupied_xy_positions(2, initial_y, 3, initial_y, 4, initial_y, 3, initial_y - 1, 0, 0, 0, 0);
		break;
	case Z_BLOCK:
		return create_occupied_xy_positions(2, initial_y - 1, 3, initial_y - 1, 3, initial_y, 4, initial_y, 0, 0, 0, 0);
		break;
		// Add other cases for different block types when needed
	}
}