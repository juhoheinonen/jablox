#include <stddef.h>

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

typedef enum
{
    EMPTY,
    WALL,    
    I_BLOCK,
    J_BLOCK,
    L_BLOCK,
    O_BLOCK,
    S_BLOCK,
    T_BLOCK,
    Z_BLOCK,
    I_BLOCK_FALLEN,
    J_BLOCK_FALLEN,
    L_BLOCK_FALLEN,
    O_BLOCK_FALLEN,
    S_BLOCK_FALLEN,
    T_BLOCK_FALLEN,
    Z_BLOCK_FALLEN        
} tile_block_type;



typedef struct
{
    int x;
    int y;
} xy_position;

// this struct is used to store the coordinates of the block that are occupied. Every block has 4 pieces.
typedef struct
{
    xy_position xy1;
    xy_position xy2;
    xy_position xy3;
    xy_position xy4;
} occupied_xy_positions;

typedef struct
{
    int x;
    int y;
    tile_block_type type;
} game_tile;

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} rotation;

typedef enum
{
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_DOWN
} direction;

typedef struct
{
    // these point to left bottom corner of the block in its original format
    xy_position current_xy_position;
    tile_block_type block_type;
    rotation rotation;
    direction direction;
    occupied_xy_positions occupied_xy_positions;
    occupied_xy_positions previous_occupied_xy_positions;
    int fast_drop;
} block;

// enum for game status, like running, game over and win
typedef enum
{
    RUNNING,
    GAME_OVER,
    WIN
} game_status;

#endif