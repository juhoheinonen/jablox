#ifndef BLOCK_POSITIONS_H
#define BLOCK_POSITIONS_H

#include "data_types.h"

occupied_xy_positions get_initial_xy_positions_by_block_type(tile_block_type block_type);
occupied_xy_positions get_new_positions(block current_block);

#endif // BLOCK_POSITIONS_H