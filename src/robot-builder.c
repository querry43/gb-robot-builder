#include <gb/gb.h>

#include "agent.h"
#include "../tile-data/background.game.map.h"
#include "../tile-data/game.tileset.h"
#include "../tile-data/gumdrop.sprite.h"

// goals:
// 1. have multiple autonomous agents navigating a surface
// 2. have one player agent navigating a surface

void draw_background();
void init_agents();
void draw_agents();
void update_agents();

UBYTE frame_counter;

#define player_agent agents[0]

void rotate_agent(const UBYTE agent) {
  if (frame_counter == 0) {
    if (agents[agent].facing_direction >= LEFT)
      agents[agent].facing_direction = DOWN;
    else
      agents[agent].facing_direction++;
    agents[agent].dirty = TRUE;
  }
}

void walk_right_agent(const UBYTE agent) {
  if (frame_counter == 0) {
    agents[agent].position.x.b.h++;
    agents[agent].dirty = TRUE;
  }
}

void run_down_agent(const UBYTE agent) {
  if (frame_counter == 0 || frame_counter == 64 || frame_counter == 128 || frame_counter == 192) {
    agents[agent].position.y.b.h++;
    agents[agent].dirty = TRUE;
  }
}

void sprint_left_agent(const UBYTE agent) {
  if (frame_counter == 0 || frame_counter == 32 || frame_counter == 64 || frame_counter == 96 || frame_counter == 128 || frame_counter == 160 || frame_counter == 192 || frame_counter == 224) {
    agents[agent].position.x.b.h--;
    agents[agent].dirty = TRUE;
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
  int i;

  for (i = 0; i != num_agents; i++) {
    agents[i].sprite_id = i * 2;
    disable_agent(i);
  }

  player_agent.enabled = TRUE;
  player_agent.sprite_id = 0;
  player_agent.sprite_sheet_offset = 0;
  player_agent.facing_direction = DOWN;
  player_agent.position.x.b.h = 16;
  player_agent.position.y.b.h = 30;
  player_agent.agent_function = rotate_agent;
  player_agent.dirty = TRUE;

  agents[1].enabled = TRUE;
  agents[1].sprite_id = 2;
  agents[1].sprite_sheet_offset = 0;
  agents[1].facing_direction = RIGHT;
  agents[1].position.x.b.h = 0;
  agents[1].position.y.b.h = 60;
  agents[1].agent_function = walk_right_agent;
  agents[1].dirty = TRUE;

  agents[2].enabled = TRUE;
  agents[2].sprite_id = 4;
  agents[2].sprite_sheet_offset = 0;
  agents[2].facing_direction = DOWN;
  agents[2].position.x.b.h = 60;
  agents[2].position.y.b.h = 60;
  agents[2].agent_function = run_down_agent;
  agents[2].dirty = TRUE;

  agents[3].enabled = TRUE;
  agents[3].sprite_id = 6;
  agents[3].sprite_sheet_offset = 0;
  agents[3].facing_direction = LEFT;
  agents[3].position.x.b.h = 0;
  agents[3].position.y.b.h = 90;
  agents[3].agent_function = sprint_left_agent;
  agents[3].dirty = TRUE;
}

void draw_agents() {
  int i;
  for (i = 0; i != num_agents; i++)
    if (agents[i].enabled || agents[i].dirty)
      draw_agent(i);
}

void update_agents() {
  int i;
  for (i = 0; i != num_agents; i++) {
    if (agents[i].enabled)
      (*agents[i].agent_function)(i);
  }
}

// vim: ts=2 sw=2
