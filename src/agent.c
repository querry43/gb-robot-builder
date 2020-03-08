#include <gb/gb.h>
#include "agent.h"

agent_t agents[num_agents];

void draw_agent(const UINT8 agent_id) {
  const direction_t facing_direction = agents[agent_id].facing_direction;
  const UINT8 sid1 = agents[agent_id].sprite_id;
  const UINT8 sid2 = agents[agent_id].sprite_id + 1;

  switch(facing_direction) {
    case DOWN:
      set_sprite_tile(sid1, agents[agent_id].sprite_sheet_offset + 0);
      set_sprite_tile(sid2, agents[agent_id].sprite_sheet_offset + 2);
      set_sprite_prop(sid1, get_sprite_prop(sid1) & ~ S_FLIPX);
      set_sprite_prop(sid2, get_sprite_prop(sid2) & ~ S_FLIPX);
      break;

    case UP:
      set_sprite_tile(sid1, agents[agent_id].sprite_sheet_offset + 4);
      set_sprite_tile(sid2, agents[agent_id].sprite_sheet_offset + 6);
      set_sprite_prop(sid1, get_sprite_prop(sid1) & ~ S_FLIPX);
      set_sprite_prop(sid2, get_sprite_prop(sid2) & ~ S_FLIPX);
      break;

    case RIGHT:
      set_sprite_tile(sid1, agents[agent_id].sprite_sheet_offset + 8);
      set_sprite_tile(sid2, agents[agent_id].sprite_sheet_offset + 10);
      set_sprite_prop(sid1, get_sprite_prop(sid1) & ~ S_FLIPX);
      set_sprite_prop(sid2, get_sprite_prop(sid2) & ~ S_FLIPX);
      break;

    case LEFT:
      set_sprite_tile(sid1, agents[agent_id].sprite_sheet_offset + 10);
      set_sprite_tile(sid2, agents[agent_id].sprite_sheet_offset + 8);
      set_sprite_prop(sid1, get_sprite_prop(sid1) | S_FLIPX);
      set_sprite_prop(sid2, get_sprite_prop(sid2) | S_FLIPX);
      break;
  }

  move_sprite(sid1, agents[agent_id].position.x.byte.high - 7, agents[agent_id].position.y.byte.high + 1);
  move_sprite(sid2, agents[agent_id].position.x.byte.high + 1, agents[agent_id].position.y.byte.high + 1);

  agents[agent_id].dirty = FALSE;
}

void disable_agent(const UINT8 agent_id) {
  agents[agent_id].enabled = FALSE;
  agents[agent_id].agent_function = null_agent;
}

void null_agent(const UINT8 agent_id) {
  (void)agent_id;
}

// vim: ts=2 sw=2
