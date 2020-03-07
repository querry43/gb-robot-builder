#include <gb/gb.h>
#include "agent.h"

agent_t agents[num_agents];

void draw_agent(const UBYTE agent) {
  const UBYTE facing_direction = agents[agent].facing_direction;
  const UBYTE sid1 = agents[agent].sprite_id;
  const UBYTE sid2 = agents[agent].sprite_id + 1;

  switch(facing_direction) {
    case DOWN:
      set_sprite_tile(sid1, agents[agent].sprite_sheet_offset + 0);
      set_sprite_tile(sid2, agents[agent].sprite_sheet_offset + 2);
      set_sprite_prop(sid1, get_sprite_prop(sid1) & ~ S_FLIPX);
      set_sprite_prop(sid2, get_sprite_prop(sid2) & ~ S_FLIPX);
      break;

    case UP:
      set_sprite_tile(sid1, agents[agent].sprite_sheet_offset + 4);
      set_sprite_tile(sid2, agents[agent].sprite_sheet_offset + 6);
      set_sprite_prop(sid1, get_sprite_prop(sid1) & ~ S_FLIPX);
      set_sprite_prop(sid2, get_sprite_prop(sid2) & ~ S_FLIPX);
      break;

    case RIGHT:
      set_sprite_tile(sid1, agents[agent].sprite_sheet_offset + 8);
      set_sprite_tile(sid2, agents[agent].sprite_sheet_offset + 10);
      set_sprite_prop(sid1, get_sprite_prop(sid1) & ~ S_FLIPX);
      set_sprite_prop(sid2, get_sprite_prop(sid2) & ~ S_FLIPX);
      break;

    case LEFT:
      set_sprite_tile(sid1, agents[agent].sprite_sheet_offset + 10);
      set_sprite_tile(sid2, agents[agent].sprite_sheet_offset + 8);
      set_sprite_prop(sid1, get_sprite_prop(sid1) | S_FLIPX);
      set_sprite_prop(sid2, get_sprite_prop(sid2) | S_FLIPX);
      break;
  }

  move_sprite(sid1, agents[agent].position.x.b.h, agents[agent].position.y.b.h);
  move_sprite(sid2, agents[agent].position.x.b.h + 8, agents[agent].position.y.b.h);

  agents[agent].dirty = FALSE;
}

void disable_agent(const UBYTE agent) {
  agents[agent].enabled = FALSE;
}

// vim: ts=2 sw=2
