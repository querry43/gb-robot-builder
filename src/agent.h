#ifndef __agent_h__
#define __agent_h__

#include "config.h"

typedef void (*agent_function_t)(const UBYTE agent);

typedef enum _direction_t {
  DOWN = 0,
  RIGHT = 1,
  UP = 2,
  LEFT = 3
} direction_t;

typedef union _coord_t {
  struct {
    UBYTE l;
    UBYTE h;
  } b;
  WORD w;
} coord_t;

typedef struct _pos_t {
  coord_t x;
  coord_t y;
} pos_t;

typedef struct _agent_t {
  UBYTE sprite_id;
  UBYTE sprite_sheet_offset;

  BOOLEAN enabled;
  BOOLEAN dirty;

  direction_t facing_direction;
  pos_t position;

  agent_function_t agent_function;
} agent_t;

extern void draw_agent(const UBYTE agent);

extern void disable_agent(const UBYTE agent);

extern agent_t agents[num_agents];

#endif

// vim: ts=2 sw=2
