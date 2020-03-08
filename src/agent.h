#ifndef __agent_h__
#define __agent_h__

#include "config.h"

#define point_to_tile(point) point >> 3

#define agent_x_pos(agent_id) agents[agent_id].position.x.byte.high
#define agent_y_pos(agent_id) agents[agent_id].position.y.byte.high

#define agent_x_tile(agent_id) point_to_tile(agents[agent_id].position.x.byte.high)
#define agent_y_tile(agent_id) point_to_tile(agents[agent_id].position.y.byte.high)

typedef void (*agent_function_t)(const UINT8 agent_id);

typedef enum _direction_t {
  DOWN = 0,
  RIGHT = 1,
  UP = 2,
  LEFT = 3
} direction_t;

typedef union _coord_t {
  struct {
    UINT8 low;
    UINT8 high;
  } byte;
  INT16 word;
} coord_t;

typedef struct _pos_t {
  coord_t x;
  coord_t y;
} pos_t;

typedef struct _box_t {
  INT8 left;
  INT8 right;
  INT8 top;
  INT8 bottom;
} box_t;

typedef struct _agent_t {
  UINT8 sprite_id;            // sprites will use sprite_id and sprite_id+1
  UINT8 sprite_sheet_offset;

  BOOLEAN enabled;
  BOOLEAN dirty;
  BOOLEAN animate;

  UINT8 sprite_offset;

  direction_t facing_direction;
  pos_t position;             // based on sprite bottom right corner

  box_t bounds;

  agent_function_t agent_function;
} agent_t;

extern void draw_agent(const UINT8 agent_id);

extern void disable_agent(const UINT8 agent_id);

extern void null_agent(const UINT8 agent_id);

extern agent_t agents[num_agents];

#endif

// vim: ts=2 sw=2
