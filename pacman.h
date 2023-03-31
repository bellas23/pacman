#pragma once

// j=x-coord
#define BOARD_SIZE_j 28
// i=y-coord
#define BOARD_SIZE_i 31

#define INKY "ᗣ"
#define BLINKY "ᗣ"
#define PINKY "ᗣ"
#define CLYDE "ᗣ"
#define POW_UP "●"
#define PAC_DOT "•"
#define PAC_UP "◷"
#define PAC_RIGHT "ᗧ"
#define PAC_LEFT "ᗤ"
#define PAC_DOWN "◶"
#define VERT_WALL
#define HORIZONTAL_WALL
#define LEFT_DOWN_CORNER
#define LEFT_UP_CORNER
#define RIGHT_DOWN_CORNER
#define RIGHT_UP_CORNER
#define LOOP_DELAY_NANO 1000000000

// Globals
int game_status = 1;
int score = 0;
int one_up = 0;
int num_dots;
int num_lives = 2;
int num_level = 1;
int speed = 3;
int num_dots_eaten;
int highscore;
char score_holder[4];
int pac_speed = 1;
int ghost_speed = 2;
int num_time_red_scatter = 0;
int num_time_orange_scatter = 0;
int num_time_cyan_scatter = 0;
int num_time_pink_scatter = 0;
int frame;
int ghost_eaten;

// direction struct
typedef enum {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT
} direction;

// Struct for power up timer
struct timeval powerup_timer;

// Struct for ghosts
typedef struct ghost_struct {
  int alive;
  int pos_x;
  int pos_y;
  int pos_start_x;
  int pos_start_y;
  int dir_x;
  int dir_y;
  float speed;
  // is the ghost contained within the starting box or not
  int out_of_box;
  int frame_when_dead;
} ghost;

// Struct for pacman
typedef struct pacman_struct {
  int pos_x;
  int pos_y;
  direction dir;
  int alive;
  int poweredup;
} Pacman;

// Struct for each spot on the board
typedef struct board_spot_struct {
  int dots;
  int powerup;
  // if spot on track is playable F = 0, T = 1
  int track;
  int is_pacman;
  int is_ghost;
} board_spot;

// Struct for fruit
typedef struct fruit_struct {
  int on_board;
  int num_fruit_eaten;
} fruits;

// Global for fruit
fruits fruit;

// Global Character Pointers
Pacman *pacman;
ghost *red;
ghost *pink;
ghost *cyan;
ghost *orange;

// Function prototypes
void set_up_print();
void print_board(board_spot **board);
void populate_board(board_spot **board);
void pacman_mover(board_spot **board);
void fruit_spawner(board_spot **board);
void print_hud();
void ghost_pacman_interaction(board_spot **board, ghost *cur_ghost);
void print_highscore();
void update_highscore();
void ghost_chase(board_spot **board, int target_x, int target_y, ghost *cur_ghost);
void ghost_frieghtened(board_spot **board, ghost *cur_ghost);
void red_ghost_mover(board_spot **board);
void orange_ghost_mover(board_spot **board);
void pink_ghost_mover(board_spot **board);
void cyan_ghost_mover(board_spot **board);
void reset_board(board_spot **board);
void ghost_scatter(int elapsed_time);
void ghost_respawn(ghost *cur_ghost, int frame);
void points_eaten_ghost();

