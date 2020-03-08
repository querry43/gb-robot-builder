#include <gb/gb.h>
#include <string.h>

#include "agent.h"
#include "../tile-data/background.game.map.h"
#include "../tile-data/game.tileset.h"
#include "../tile-data/gumdrop.sprite.h"

// goals:
// 1. have multiple autonomous agents navigating a surface
// 2. have one player agent navigating a surface
// 3. have all agents interact with obstacles
// 4. have agents interact with eachother

// this is a boring test boundary
// 11111111
// 11000011
// 11000011
// 11111111
UINT8 background_game_map_mask[] = {
0xff, 0xff, 0xff, 0xff, // y = 0
0xff, 0xff, 0xff, 0xff, // y = 1
0xc2, 0x00, 0x00, 0x03,
0xc2, 0x00, 0x00, 0x03,
0xc2, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xfc, 0x00, 0x00, 0x03, // y = 8
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xc0, 0x00, 0x00, 0x03,
0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, // y = 31
};

void draw_background();
void init_agents();
void draw_agents();
void update_agents();

UINT8 frame_counter;

#define player_agent agents[0]

BOOLEAN tile_is_occupied(const UINT8 x_tile, const UINT8 y_tile) {
  const UINT8 x_offset = x_tile >> 3;
  const UINT8 y_offset = y_tile << 2;

  const UINT8 mask_offset = x_offset + y_offset;
  const UINT8 local_mask = background_game_map_mask[mask_offset];
  const UINT8 x_tile_lsb = x_tile & 0x07;
  const UINT8 x_tile_bitmask = 0x80ul >> x_tile_lsb;

  return local_mask & x_tile_bitmask;
}

void player_control_agent(const UINT8 agent_id) {
  UINT8 j;
  direction_t new_facing_direction = agents[agent_id].facing_direction;
  pos_t new_position;

  UINT8 new_x_min_tile;
  UINT8 new_x_mid_tile;
  UINT8 new_x_max_tile;
  UINT8 new_y_min_tile;
  UINT8 new_y_mid_tile;
  UINT8 new_y_max_tile;

  BOOLEAN moving_up = FALSE;
  BOOLEAN moving_left = FALSE;

  new_position.x.word = agents[agent_id].position.x.word;
  new_position.y.word = agents[agent_id].position.y.word;

  // consider only executing this block every N frames for performance
  j = joypad();

  if (j & J_RIGHT) {
    new_facing_direction = RIGHT;
    new_position.x.word += player_speed;
  } else if (j & J_LEFT) {
    moving_left = TRUE;
    new_facing_direction = LEFT;
    new_position.x.word -= player_speed;
  }

  if (j & J_DOWN) {
    new_facing_direction = DOWN;
    new_position.y.word += player_speed;
  } else if (j & J_UP) {
    moving_up = TRUE;
    new_facing_direction = UP;
    new_position.y.word -= player_speed;
  }

  new_x_min_tile = point_to_tile(new_position.x.byte.high + agents[agent_id].bounds.left);
  new_x_mid_tile = point_to_tile(new_position.x.byte.high + agents[agent_id].bounds.left + 7);
  new_x_max_tile = point_to_tile(new_position.x.byte.high + agents[agent_id].bounds.right);

  new_y_min_tile = point_to_tile(new_position.y.byte.high + agents[agent_id].bounds.bottom);
  new_y_mid_tile = point_to_tile(new_position.y.byte.high + agents[agent_id].bounds.bottom - 7);
  new_y_max_tile = point_to_tile(new_position.y.byte.high + agents[agent_id].bounds.top);


  if (moving_left) {
    // check bottom left, middle left, top left
    if (
        tile_is_occupied(new_x_min_tile, new_y_min_tile)
        || tile_is_occupied(new_x_min_tile, new_y_mid_tile)
        || tile_is_occupied(new_x_min_tile, new_y_max_tile))
      new_position.x.word = agents[agent_id].position.x.word;
  } else {
    // check bottom right, middle right, top right
    if (
        tile_is_occupied(new_x_max_tile, new_y_min_tile)
        || tile_is_occupied(new_x_max_tile, new_y_mid_tile)
        || tile_is_occupied(new_x_max_tile, new_y_max_tile))
      new_position.x.word = agents[agent_id].position.x.word;
  }

  new_x_min_tile = point_to_tile(new_position.x.byte.high + agents[agent_id].bounds.left);
  new_x_mid_tile = point_to_tile(new_position.x.byte.high + agents[agent_id].bounds.left + 7);
  new_x_max_tile = point_to_tile(new_position.x.byte.high + agents[agent_id].bounds.right);

  if (moving_up) {
    // check top left, top middle, top right
    if (
        tile_is_occupied(new_x_min_tile, new_y_max_tile)
        || tile_is_occupied(new_x_mid_tile, new_y_max_tile)
        || tile_is_occupied(new_x_max_tile, new_y_max_tile))
      new_position.y.word = agents[agent_id].position.y.word;
  } else {
    // check bottom left, bottom middle, bottom right
    if (
        tile_is_occupied(new_x_min_tile, new_y_min_tile)
        || tile_is_occupied(new_x_mid_tile, new_y_min_tile)
        || tile_is_occupied(new_x_max_tile, new_y_min_tile))
      new_position.y.word = agents[agent_id].position.y.word;
  }

  agents[agent_id].facing_direction = new_facing_direction;
  agents[agent_id].position.x.word = new_position.x.word;
  agents[agent_id].position.y.word = new_position.y.word;
  agents[agent_id].dirty = TRUE;
}

void rotate_agent(const UINT8 agent_id) {
  if (frame_counter == 0) {
    if (agents[agent_id].facing_direction >= LEFT)
      agents[agent_id].facing_direction = DOWN;
    else
      agents[agent_id].facing_direction++;
    agents[agent_id].dirty = TRUE;
  }
}

void walk_right_agent(const UINT8 agent_id) {
  if (frame_counter == 0) {
    agent_x_pos(agent_id)++;
    agents[agent_id].dirty = TRUE;
  }
}

void run_down_agent(const UINT8 agent_id) {
  if (frame_counter == 0 || frame_counter == 64 || frame_counter == 128 || frame_counter == 192) {
    agent_y_pos(agent_id)++;
    agents[agent_id].dirty = TRUE;
  }
}

void sprint_left_agent(const UINT8 agent_id) {
  if (frame_counter == 0 || frame_counter == 32 || frame_counter == 64 || frame_counter == 96 || frame_counter == 128 || frame_counter == 160 || frame_counter == 192 || frame_counter == 224) {
    agent_x_pos(agent_id)--;
    agents[agent_id].dirty = TRUE;
  }
}

int main() {
  disable_interrupts();
  DISPLAY_OFF;
  HIDE_BKG;
  HIDE_SPRITES;
  HIDE_WIN;

  set_bkg_data(0, 82, game_tileset);
  draw_background();

  SPRITES_8x16;

  set_sprite_data(0, 12, gumdrop_sprite);

  init_agents();

  add_VBL(draw_agents);

  SHOW_BKG;
  HIDE_WIN;
  SHOW_SPRITES;
  DISPLAY_ON;
  enable_interrupts();

  for (;;) {
    frame_counter++;
    update_agents();
  }
}

void draw_background() {
  set_bkg_tiles(0, 0, 32, 32, background_game_map);
}

void init_agents() {
  UINT8 agent_id;

  for (agent_id = 0; agent_id != num_agents; agent_id++) {
    agents[agent_id].sprite_id = agent_id * 2;
    disable_agent(agent_id);
  }

  player_agent.enabled = TRUE;
  player_agent.sprite_id = 0;
  player_agent.sprite_sheet_offset = 0;
  player_agent.facing_direction = DOWN;
  player_agent.position.x.byte.high = 30;
  player_agent.position.y.byte.high = 40;
  player_agent.bounds.left = -13;
  player_agent.bounds.right = -1;
  player_agent.bounds.top = -15;
  player_agent.agent_function = player_control_agent;
  player_agent.dirty = TRUE;

  agents[1].enabled = TRUE;
  agents[1].sprite_id = 2;
  agents[1].sprite_sheet_offset = 0;
  agents[1].facing_direction = RIGHT;
  agent_x_pos(1) = 0;
  agent_y_pos(1) = 60;
  agents[1].agent_function = walk_right_agent;
  agents[1].dirty = TRUE;

  agents[2].enabled = TRUE;
  agents[2].sprite_id = 4;
  agents[2].sprite_sheet_offset = 0;
  agents[2].facing_direction = DOWN;
  agent_x_pos(2) = 60;
  agent_y_pos(2) = 60;
  agents[2].agent_function = run_down_agent;
  agents[2].dirty = TRUE;

  agents[3].enabled = TRUE;
  agents[3].sprite_id = 6;
  agents[3].sprite_sheet_offset = 0;
  agents[3].facing_direction = LEFT;
  agent_x_pos(3) = 0;
  agent_y_pos(3) = 90;
  agents[3].agent_function = sprint_left_agent;
  agents[3].dirty = TRUE;
}

void draw_agents() {
  UINT8 agent_id;
  for (agent_id = 0; agent_id != num_agents; agent_id++)
    if (agents[agent_id].enabled && agents[agent_id].dirty)
      draw_agent(agent_id);
}

void update_agents() {
  UINT8 agent_id;
  for (agent_id = 0; agent_id != num_agents; agent_id++)
    if (agents[agent_id].enabled)
      (*agents[agent_id].agent_function)(agent_id);
}

// vim: ts=2 sw=2
