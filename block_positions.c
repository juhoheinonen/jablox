#include "block_positions.h"
#include "data_types.h"
occupied_xy_positions create_occupied_xy_positions(xy_position xy1, xy_position xy2, xy_position xy3, xy_position xy4, xy_position xy5, xy_position xy6)
{
    occupied_xy_positions positions;
    positions.xy1 = xy1;
    positions.xy2 = xy2;
    positions.xy3 = xy3;
    positions.xy4 = xy4;
    positions.xy5 = xy5;
    positions.xy6 = xy6;
    return positions;
}

occupied_xy_positions get_initial_xy_positions_by_block_type(tile_block_type block_type)
{
    int initial_y = 10;

    switch (block_type)
    {
    case I_BLOCK:
        return create_occupied_xy_positions((xy_position){2, initial_y}, (xy_position){3, initial_y}, (xy_position){4, initial_y}, (xy_position){5, initial_y}, (xy_position){0, 0}, (xy_position){0, 0});
        break;
    case J_BLOCK:
        return create_occupied_xy_positions((xy_position){2, initial_y - 1}, (xy_position){2, initial_y}, (xy_position){3, initial_y}, (xy_position){4, initial_y}, (xy_position){0, 0}, (xy_position){0, 0});
        break;
    case L_BLOCK:
        return create_occupied_xy_positions((xy_position){2, initial_y}, (xy_position){3, initial_y}, (xy_position){4, initial_y}, (xy_position){4, initial_y - 1}, (xy_position){0, 0}, (xy_position){0, 0});
        break;
    case O_BLOCK:
        return create_occupied_xy_positions((xy_position){2, initial_y}, (xy_position){3, initial_y}, (xy_position){2, initial_y - 1}, (xy_position){3, initial_y - 1}, (xy_position){0, 0}, (xy_position){0, 0});
        break;
    case S_BLOCK:
        return create_occupied_xy_positions((xy_position){2, initial_y}, (xy_position){3, initial_y}, (xy_position){3, initial_y - 1}, (xy_position){4, initial_y - 1}, (xy_position){0, 0}, (xy_position){0, 0});
        break;
    case T_BLOCK:
        return create_occupied_xy_positions((xy_position){2, initial_y}, (xy_position){3, initial_y}, (xy_position){4, initial_y}, (xy_position){3, initial_y - 1}, (xy_position){0, 0}, (xy_position){0, 0});
        break;
    case Y_BLOCK:
        return create_occupied_xy_positions((xy_position){2, initial_y - 1}, (xy_position){2, initial_y}, (xy_position){3, initial_y}, (xy_position){4, initial_y}, (xy_position){5, initial_y - 1}, (xy_position){3, initial_y + 1});
        break;
    case Z_BLOCK:
        return create_occupied_xy_positions((xy_position){2, initial_y - 1}, (xy_position){3, initial_y - 1}, (xy_position){3, initial_y}, (xy_position){4, initial_y}, (xy_position){0, 0}, (xy_position){0, 0});
        break;
        // Add other cases for different block types when needed
    }
}

occupied_xy_positions get_new_positions(block current_block)
{
	occupied_xy_positions positions = current_block.occupied_xy_positions;

	occupied_xy_positions new_positions;
	switch (current_block.block_type)
	{
	case L_BLOCK:
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
	case S_BLOCK:
		if (current_block.rotation == RIGHT || current_block.rotation == LEFT)
		{
			new_positions.xy1.x = positions.xy1.x;
			new_positions.xy1.y = positions.xy1.y - 1;
			new_positions.xy2.x = positions.xy2.x - 1;
			new_positions.xy2.y = positions.xy2.y;
			new_positions.xy3.x = positions.xy3.x;
			new_positions.xy3.y = positions.xy3.y + 1;
			new_positions.xy4.x = positions.xy4.x - 1;
			new_positions.xy4.y = positions.xy4.y + 2;
		}
		else if (current_block.rotation == DOWN || current_block.rotation == UP)
		{
			new_positions.xy1.x = positions.xy1.x;
			new_positions.xy1.y = positions.xy1.y + 1;
			new_positions.xy2.x = positions.xy2.x + 1;
			new_positions.xy2.y = positions.xy2.y;
			new_positions.xy3.x = positions.xy3.x;
			new_positions.xy3.y = positions.xy3.y - 1;
			new_positions.xy4.x = positions.xy4.x + 1;
			new_positions.xy4.y = positions.xy4.y - 2;
		}
		break;
	case T_BLOCK:
		// return create_occupied_xy_positions(2, initial_y, 3, initial_y, 4, initial_y, 3, initial_y - 1);
		if (current_block.rotation == RIGHT)
		{
			new_positions.xy1.x = positions.xy1.x + 1;
			new_positions.xy1.y = positions.xy1.y - 1;
			new_positions.xy2.x = positions.xy2.x;
			new_positions.xy2.y = positions.xy2.y;
			new_positions.xy3.x = positions.xy3.x - 1;
			new_positions.xy3.y = positions.xy3.y + 1;
			new_positions.xy4.x = positions.xy4.x + 1;
			new_positions.xy4.y = positions.xy4.y + 1;
		}
		else if (current_block.rotation == DOWN)
		{
			new_positions.xy1.x = positions.xy1.x - 1;
			new_positions.xy1.y = positions.xy1.y + 1;
			new_positions.xy2.x = positions.xy2.x;
			new_positions.xy2.y = positions.xy2.y;
			new_positions.xy3.x = positions.xy3.x + 1;
			new_positions.xy3.y = positions.xy3.y - 1;
			new_positions.xy4.x = positions.xy4.x - 1;
			new_positions.xy4.y = positions.xy4.y + 1;
		}
		else if (current_block.rotation == LEFT)
		{
			new_positions.xy1.x = positions.xy1.x + 1;
			new_positions.xy1.y = positions.xy1.y - 1;
			new_positions.xy2.x = positions.xy2.x;
			new_positions.xy2.y = positions.xy2.y;
			new_positions.xy3.x = positions.xy3.x - 1;
			new_positions.xy3.y = positions.xy3.y + 1;
			new_positions.xy4.x = positions.xy4.x - 1;
			new_positions.xy4.y = positions.xy4.y - 1;
		}
		else if (current_block.rotation == UP)
		{
			new_positions.xy1.x = positions.xy1.x - 1;
			new_positions.xy1.y = positions.xy1.y + 1;
			new_positions.xy2.x = positions.xy2.x;
			new_positions.xy2.y = positions.xy2.y;
			new_positions.xy3.x = positions.xy3.x + 1;
			new_positions.xy3.y = positions.xy3.y - 1;
			new_positions.xy4.x = positions.xy4.x + 1;
			new_positions.xy4.y = positions.xy4.y - 1;
		}
		break;
	case Z_BLOCK:
		if (current_block.rotation == RIGHT || current_block.rotation == LEFT)
		{
			new_positions.xy1.x = positions.xy1.x;
			new_positions.xy1.y = positions.xy1.y + 2;
			new_positions.xy2.x = positions.xy2.x - 1;
			new_positions.xy2.y = positions.xy2.y + 1;
			new_positions.xy3.x = positions.xy3.x;
			new_positions.xy3.y = positions.xy3.y;
			new_positions.xy4.x = positions.xy4.x - 1;
			new_positions.xy4.y = positions.xy4.y - 1;
		}
		else if (current_block.rotation == DOWN || current_block.rotation == UP)
		{
			new_positions.xy1.x = positions.xy1.x;
			new_positions.xy1.y = positions.xy1.y - 2;
			new_positions.xy2.x = positions.xy2.x + 1;
			new_positions.xy2.y = positions.xy2.y - 1;
			new_positions.xy3.x = positions.xy3.x;
			new_positions.xy3.y = positions.xy3.y;
			new_positions.xy4.x = positions.xy4.x + 1;
			new_positions.xy4.y = positions.xy4.y + 1;
		}
		break;
	}

	return new_positions;
}