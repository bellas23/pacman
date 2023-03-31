#include <locale.h>
#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <uchar.h>
#include <unistd.h>
#include <wchar.h>
#include <math.h>
#include "pacman.h"

// Calls all neccessary curses and unicode set up
void set_up_print() {
  setlocale(LC_CTYPE, "");
  initscr();
  raw();
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);

  // Creating more vibrant yellow
  init_color(COLOR_YELLOW, 725, 725, 0);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);

  init_pair(3, COLOR_GREEN, COLOR_BLACK);

  init_color(8, 1000, 647, 0);
  init_pair(8, 8, COLOR_BLACK);

  init_pair(4, COLOR_BLUE, COLOR_BLACK);

  // Creating pink
  init_color(COLOR_MAGENTA, 1000, 750, 796);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);

  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_BLACK);

  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);
}

// Looks at condition for each board spot and prints to screen
void print_board(board_spot **board) {
  FILE *map = fopen("map", "r");

  // Look at map file, and reprint walls using unicode characters corresponding
  // to map character
  // Each board spot gets iterated through, checking conditions contained within
  // the struct for that board position
  for (int i = 0; i < BOARD_SIZE_i; i++) {
    char map_string[BOARD_SIZE_j + 2];
    fgets(map_string, BOARD_SIZE_j + 2, map);
    for (int j = 0; j < BOARD_SIZE_j; j++) {
      // Reprints the borders
      switch (map_string[j]) {
      case '_':
        attron(COLOR_PAIR(4));
        mvprintw(i, j, "═");
        break;
      case '|':
        attron(COLOR_PAIR(4));
        mvprintw(i, j, "║");
        break;
      case '1':
        attron(COLOR_PAIR(4));
        mvprintw(i, j, "╔");
        break;

      case '2':
        attron(COLOR_PAIR(4));
        mvprintw(i, j, "╗");
        break;

      case '3':
        attron(COLOR_PAIR(4));
        mvprintw(i, j, "╚");
        break;
      case '4':
        attron(COLOR_PAIR(4));
        mvprintw(i, j, "╝");
        break;
      }
      // Prints movement of ghosts and pacman to board based off values
      // contained within structs Pacman comes first that way if ghost and
      // Pacman are in same location, ghost gets printed, not Pacman
      if ((pacman->pos_x == j) && (pacman->pos_y == i)) {
        attron(COLOR_PAIR(2));
        if (pacman->dir == KEY_RIGHT)
          mvprintw(i, j, "%s", PAC_RIGHT);
        else if (pacman->dir == KEY_LEFT)
          mvprintw(i, j, "%s", PAC_LEFT);
        else if (pacman->dir == KEY_DOWN)
          mvprintw(i, j, "%s", PAC_DOWN);
        else if (pacman->dir == KEY_UP)
          mvprintw(i, j, "%s", PAC_UP);
        else mvprintw(i, j, "%s", PAC_RIGHT);
      }
      // Printing dots, powerups, and fruit to board
      if (board[i][j].track == 1) {
        // Now look to print dots and powers
        if (board[i][j].powerup == 1) {
          attron(COLOR_PAIR(2));
          mvprintw(i, j, "%s", POW_UP);
        } else if (board[i][j].dots == 1) {
          attron(COLOR_PAIR(2));
          mvprintw(i, j, "%s", PAC_DOT);
        }
      }
      attron(COLOR_PAIR(4));

      //printing fruit to board
      if (fruit.on_board == 1) mvprintw(17, 13, "🍒");
      
      // Now print ghosts
      if ((red->pos_x == j) && (red->pos_y == i)) {
        if (red->alive >= 1) attron(COLOR_PAIR(1));
        else if (red->alive == 0) {
          attron(COLOR_PAIR(4));
        }
        mvprintw(i, j, "%s", BLINKY);
        board[i][j].is_ghost = 1;
      } 
      else if ((orange->pos_x == j) && (orange->pos_y == i)) {
        if (orange->alive >= 1) attron(COLOR_PAIR(8));
        else if (orange->alive == 0) {
          attron(COLOR_PAIR(4));
        }
        mvprintw(i, j, "%s", CLYDE);
        board[i][j].is_ghost = 1;
      } 
      else if ((cyan->pos_x == j) && (cyan->pos_y == i)) {
        if (cyan->alive >= 1) attron(COLOR_PAIR(6));
        else if (cyan->alive == 0) {
          attron(COLOR_PAIR(4));
        }
        mvprintw(i, j, "%s", INKY);
        board[i][j].is_ghost = 1;
      } 
      else if ((pink->pos_x == j) && (pink->pos_y == i)) {
        if (pink->alive >= 1) attron(COLOR_PAIR(5));
        else if (pink->alive == 0) {
          attron(COLOR_PAIR(4));
        }
        mvprintw(i, j, "%s", PINKY);
        board[i][j].is_ghost = 1;
      }
    }
  }
  fclose(map);
}

// Initializes the board based off of map file
void populate_board(board_spot **board) {
  FILE *map = fopen("map", "r");
  // Initializes our board by looking through map file based off of character
  // value at each spot
  for (int i = 0; i < BOARD_SIZE_i; i++) {
    char map_string[BOARD_SIZE_j + 2];
    fgets(map_string, BOARD_SIZE_j + 2, map);
    for (int j = 0; j < BOARD_SIZE_j; j++) {
      switch (map_string[j]) {
      case '_':
        board[i][j].track = 0;
        break;
      case '|':
        board[i][j].track = 0;
        break;

      case '1':
        board[i][j].track = 0;
        break;

      case '2':
        board[i][j].track = 0;
        break;

      case '3':
        board[i][j].track = 0;
        break;

      case '4':
        board[i][j].track = 0;
        break;

      case 'P':
        board[i][j].track = 1;
        pacman->pos_x = j;
        pacman->pos_y = i;
        break;

      case 'q':
        board[i][j].track = 1;
        board[i][j].powerup = 1;
        num_dots += 1;
        break;

      case 'r':
        board[i][j].track = 1;
        red->pos_x = j;
        red->pos_y = i;
        break;

      case 'c':
        board[i][j].track = 1;
        cyan->pos_x = j;
        cyan->pos_y = i;
        break;

      case 'o':
        board[i][j].track = 1;
        orange->pos_x = j;
        orange->pos_y = i;
        break;

      case 'p':
        board[i][j].track = 1;
        pink->pos_x = j;
        pink->pos_y = i;
        break;

      case 'e':
        board[i][j].track = 0;
        board[i][j].dots = 0;
        break;

      case 'f':
        board[i][j].track = 1;
        board[i][j].dots = 0;
        break;

      default:
        board[i][j].track = 1;
        board[i][j].dots = 1;
        num_dots += 1;
        break;
      }
    }
  }
  fclose(map);
}

// Moves Pacman based on user inputs, if no user input, Pacman moves in last direction
void pacman_mover(board_spot **board) {
  int input = 0;
  input = getch();
  if ((input == KEY_RIGHT) || (input == KEY_LEFT) || (input == KEY_UP) ||
      (input == KEY_DOWN))
    pacman->dir = input;
  if (pacman->dir == KEY_DOWN) {
    if (board[pacman->pos_y + 1][pacman->pos_x].track == 1)
      pacman->pos_y += 1;
  }
  if (pacman->dir == KEY_UP) {
    if (board[pacman->pos_y - 1][pacman->pos_x].track == 1)
      pacman->pos_y -= 1;
  }
  if (pacman->dir == KEY_RIGHT) {
    if (board[pacman->pos_y][pacman->pos_x + 1].track == 1)
      pacman->pos_x += 1;
    else if ((pacman->pos_y == 14) && (pacman->pos_x == 27))
      pacman->pos_x = 0;
  }
  if (pacman->dir == KEY_LEFT) {
    if (board[pacman->pos_y][pacman->pos_x - 1].track == 1)
      pacman->pos_x -= 1;
    else if ((pacman->pos_y == 14) && (pacman->pos_x == 0))
      pacman->pos_x = 27;
  }

  // Consuming dots and powerups w/ increasing score
  if (board[pacman->pos_y][pacman->pos_x].dots == 1) {
    board[pacman->pos_y][pacman->pos_x].dots = 0;
    score += 10; one_up += 10;
    num_dots -= 1; num_dots_eaten += 1;
  }

  // Allow Pacman to eat powerups
  if (board[pacman->pos_y][pacman->pos_x].powerup == 1) {
    board[pacman->pos_y][pacman->pos_x].powerup = 0;
    red->alive = 0; red->dir_x = 0; red->dir_y= 0;
    pink->alive = 0; pink->dir_x=0; pink->dir_y=0;
    cyan->alive = 0; cyan->dir_x=0; cyan->dir_y=0;
    orange->alive = 0; orange->dir_x=0; orange->dir_y=0;
    score += 50; one_up += 50;
    num_dots -= 1;
    pacman->poweredup = 1;
    gettimeofday(&powerup_timer, NULL);
    
  }
  // Allow Pac Man to eat the fruits
  if ((pacman->pos_x == 13) && (pacman->pos_y == 17)) {
    if (fruit.on_board == 1) {
      fruit.num_fruit_eaten += 1;
      score += 100; one_up += 100;
    }
    fruit.on_board = 0;
  }
}

// Function for ghost Pacman interactions
void ghost_pacman_interaction(board_spot **board, ghost* cur_ghost) {

  bool on_same_spot = (pacman->pos_x == cur_ghost->pos_x) && (pacman->pos_y == cur_ghost->pos_y);
  
  if (on_same_spot) {
    
    // If ghost is in chase mode, Pacman loses a life
    if (cur_ghost->alive >= 1 && num_lives > 0) {
      pacman->alive = 2;
      num_lives -= 1;
    }
      
    // If this was pacman's last life, game gets set to over
    else if (cur_ghost->alive >= 1) pacman->alive = 0;

    // If ghost is in frieghtened mode
    else if (cur_ghost->alive == 0) {
      cur_ghost->pos_x = cur_ghost->pos_start_x;
      cur_ghost->pos_y = cur_ghost->pos_start_y;
      cur_ghost->out_of_box = 0; 
      cur_ghost->frame_when_dead = frame;
      ghost_eaten ++;
      points_eaten_ghost();
    }
  }
}

// Creates HUD
void print_hud() {
  // Printing border
  mvprintw(0, 29, "╔");
  for (int i = 30; i < 50; i++)
    mvprintw(0, i, "═");
  mvprintw(0, 50, "╗");
  for (int i = 1; i < 8; i++)
    mvprintw(i, 29, "║");
  mvprintw(8, 29, "╚");
  for (int i = 30; i < 50; i++)
    mvprintw(8, i, "═");
  mvprintw(8, 50, "╝");
  for (int i = 1; i < 8; i++)
    mvprintw(i, 50, "║");

  // Printing scores/level
  attron(COLOR_PAIR(7));
  mvprintw(3, 30, "Score: ");
  mvprintw(3, 36, "%d", score);
  mvprintw(1, 30, "Highscore:");
  print_highscore();
  mvprintw(6, 30, "Level: ");
  mvprintw(6, 36, "%d", num_level);

  // Printing lives
  attron(COLOR_PAIR(2));
  // Seeing if you have earned an extra life
    if(one_up == 5000) {
       num_lives += 1; one_up = 0;
     }
    for (int i = 0; i < num_lives; i++) mvprintw(4, 30 + i, "%s", PAC_RIGHT);
  // Printing fruit eaten
  for (int i = 0; i < fruit.num_fruit_eaten; i++) mvprintw(7, 30 + 2*i, "🍒");

  
}

// Print highscore
void print_highscore() {
  // Reading last highscore
  FILE *high_score = fopen("highscore", "r");
  fscanf(high_score, "%d", &highscore);
  mvprintw(1, 40, "%d", highscore);
  fscanf(high_score, "\n%s", score_holder);
  mvprintw(1, 46, "%s", score_holder);
  fclose(high_score);
  // Writing new highscore
  FILE *higher_score = fopen("highscore", "w");
  if(game_status==0){
    if (score > highscore) {
      fprintf(higher_score, "%d", score);
      printf("New highscore: %d\n", score);
      sleep(2);
      printf("Enter your initials: ");
      scanf("%s", score_holder);
      fprintf(higher_score, "\n%s", score_holder);
    }
    else {
      fprintf(higher_score, "%d", highscore);
      fprintf(higher_score, "\n%s", score_holder);
    }
  }
  else {
    fprintf(higher_score, "%d", highscore);
    fprintf(higher_score, "\n%s", score_holder);
  }
  fclose(higher_score);
}

// Moves ghosts in chase mode
void ghost_chase(board_spot **board, int target_x, int target_y, ghost* cur_ghost) {
  int dist_x = target_x - cur_ghost->pos_x; 
  int dist_y = target_y - cur_ghost->pos_y;

  int y = dist_y / abs(dist_y);
  int x = dist_x / abs(dist_x);
  
  int new_pos_x = cur_ghost->pos_x + x; 
  int new_pos_y = cur_ghost->pos_y + y;

  int new_dist_x = pow(new_pos_x - target_x, 2) + pow(cur_ghost->pos_y - target_y, 2);
  int new_dist_y = pow(new_pos_y - target_y, 2) + pow(cur_ghost->pos_x - target_x, 2);

  if (cur_ghost->pos_x == 0 && cur_ghost->pos_y == 14 && cur_ghost->dir_x == -1) cur_ghost->pos_x = 27;
  else if (cur_ghost->pos_x == 27 && cur_ghost->pos_y == 14 && cur_ghost->dir_x == 1) cur_ghost->pos_x = 0;

  else if (new_dist_y < new_dist_x) {
    if (board[cur_ghost->pos_y + y][cur_ghost->pos_x].track == 1 && cur_ghost->dir_y != -y) {
      cur_ghost->pos_y += y; cur_ghost->dir_x = 0; cur_ghost->dir_y = y; 
    }
    else if (board[cur_ghost->pos_y][cur_ghost->pos_x + x].track == 1 && cur_ghost->dir_x != -x) {
      cur_ghost->pos_x += x; cur_ghost->dir_x = x; cur_ghost->dir_y = 0;
    }
    else if (board[cur_ghost->pos_y - y][cur_ghost->pos_x].track == 1 && cur_ghost->dir_y != y) {
      cur_ghost->pos_y -= y; cur_ghost->dir_x = 0; cur_ghost->dir_y = -y;
    }
    else if (board[cur_ghost->pos_y][cur_ghost->pos_x - x].track == 1 && cur_ghost->dir_x != x) {
      cur_ghost->pos_x -= x; cur_ghost->dir_x = -x; cur_ghost->dir_y = 0;
    }
  }
  else if (new_dist_y >= new_dist_x) {
    if (board[cur_ghost->pos_y][cur_ghost->pos_x + x].track == 1 && cur_ghost->dir_x != -x) {
      cur_ghost->pos_x += x; cur_ghost->dir_x = x; cur_ghost->dir_y = 0;
    }
    else if (board[cur_ghost->pos_y + y][cur_ghost->pos_x].track == 1 && cur_ghost->dir_y != -y) {
      cur_ghost->pos_y += y; cur_ghost->dir_x = 0; cur_ghost->dir_y = y;
    }
    else if (board[cur_ghost->pos_y - y][cur_ghost->pos_x].track == 1 && cur_ghost->dir_y != y) {
      cur_ghost->pos_y -= y; cur_ghost->dir_x = 0; cur_ghost->dir_y = -y;
    }
    else if (board[cur_ghost->pos_y][cur_ghost->pos_x - x].track == 1 && cur_ghost->dir_x != x) {
     cur_ghost->pos_x -= x; cur_ghost->dir_x = -x; cur_ghost->dir_y = 0;
    }
  }
}

// Moves ghosts in frieghten mode
void ghost_frieghtened(board_spot **board, ghost* cur_ghost) {
  int dist_x = pacman->pos_x - cur_ghost->pos_x; 
  int dist_y = pacman->pos_y - cur_ghost->pos_y;

  int y = dist_y / abs(dist_y);
  int x = dist_x / abs(dist_x);
  
  int new_pos_x = cur_ghost->pos_x + x; 
  int new_pos_y = cur_ghost->pos_y + y;

  int new_dist_x = pow(new_pos_x - pacman->pos_x, 2) + pow(cur_ghost->pos_y - pacman->pos_y, 2);
  int new_dist_y = pow(new_pos_y - pacman->pos_y, 2) + pow(cur_ghost->pos_x - pacman->pos_x, 2);

  // Check if ghost is on crossing bridge position, make him cross bridge
  if (cur_ghost->pos_x == 0 && cur_ghost->pos_y == 14 && cur_ghost->dir_x == -1) cur_ghost->pos_x = 27;
  else if (cur_ghost->pos_x == 27 && cur_ghost->pos_y == 14 && cur_ghost->dir_x == 1) cur_ghost->pos_x = 0;

  // Algorithm for ghost movement based on which movement minimizes distance between ghost and Pacman, if those aren't available, will take a direction
  else if ((new_dist_y <= new_dist_x)) {
    if (board[cur_ghost->pos_y - y][cur_ghost->pos_x].track == 1 && cur_ghost->dir_y != y) {
      cur_ghost->pos_y -= y; cur_ghost->dir_x = 0; cur_ghost->dir_y = -y; 
    }
    else if (board[cur_ghost->pos_y][cur_ghost->pos_x - x].track == 1 && cur_ghost->dir_x != x){
      cur_ghost->pos_x -= x; cur_ghost->dir_x = -x; cur_ghost->dir_y = 0;
    }
    else if (board[cur_ghost->pos_y + y][cur_ghost->pos_x].track == 1 && cur_ghost->dir_y != -y) {
      cur_ghost->pos_y += y; cur_ghost->dir_x = 0; cur_ghost->dir_y = y;
    }
    else if (board[cur_ghost->pos_y][cur_ghost->pos_x + x].track == 1 && cur_ghost->dir_x != -x){
      cur_ghost->pos_x += x; cur_ghost->dir_x = x; cur_ghost->dir_y = 0;
    }
  }
  else if ((new_dist_y > new_dist_x)) {
    if (board[cur_ghost->pos_y][cur_ghost->pos_x - x].track == 1 && cur_ghost->dir_x != x) {
      cur_ghost->pos_x -= x; cur_ghost->dir_x = -x; cur_ghost->dir_y = 0;
    }
    else if (board[cur_ghost->pos_y - y][cur_ghost->pos_x].track == 1 && cur_ghost->dir_y != y) {
      cur_ghost->pos_y -= y; cur_ghost->dir_x = 0; cur_ghost->dir_y = -y;
    }
    else if (board[cur_ghost->pos_y + y][cur_ghost->pos_x].track == 1 && cur_ghost->dir_y != -y) {
      cur_ghost->pos_y += y; cur_ghost->dir_x = 0; cur_ghost->dir_y = y;
    }
    else if (board[cur_ghost->pos_y][cur_ghost->pos_x + x].track == 1 && cur_ghost->dir_x != -x) {
      cur_ghost->pos_x += x; cur_ghost->dir_x = x; cur_ghost->dir_y = 0;
    }
  }
}

// Calculates Red Ghost target tile
void red_ghost_mover(board_spot **board) {
  // Red Ghost is currently in frieghtened mode
  if (red->alive == 0) {
    ghost_frieghtened(board, red);
  }
  // Red Ghost is currently in chase mode, currently
  else if (red->alive == 1) ghost_chase(board, pacman->pos_x, pacman->pos_y, red);
  // Red Ghost is currently in scatter mode
  else if (red->alive == 2) ghost_chase(board, 27, 1, red);
  
  ghost_pacman_interaction(board, red);
}

// Calculate Orange Ghost target tile
void orange_ghost_mover(board_spot **board) {
  // Orange moves like Red unless is within 5 tiles of Pacman, then scatters to corner
  // Calculate distance between Orange Ghost and Pacman
  int dist = pow(orange->pos_x - pacman->pos_x,2) + pow(orange->pos_y - pacman->pos_y, 2);

  // Ghost is in chase mode
  if (orange->alive == 1) {
    if (dist >= 64) ghost_chase(board, pacman->pos_x, pacman->pos_y, orange);
    else ghost_chase(board, 2, 1, orange);
  }
  // Ghost is frieghtened
  else if (orange->alive == 0) ghost_frieghtened(board, orange);
  //Ghost is in scatter mode
  else if (orange->alive == 2) ghost_chase(board, 2, 1, orange);
  
  ghost_pacman_interaction(board, orange);
}

// Calculates Pink Ghost target tile
void pink_ghost_mover(board_spot **board) {
  // The Pink Ghost attempts to move to the space four blocks ahead of Pacman in his current direction
  
  // Calculating Pink target tile
  int target_x = pacman->pos_x; int target_y = pacman->pos_y;
  if (pacman->dir == KEY_RIGHT) target_x = pacman->pos_x + 4;
  if (pacman->dir == KEY_LEFT) target_x = pacman->pos_x - 4;
  if (pacman->dir == KEY_UP) target_y = pacman->pos_y - 4;
  if (pacman->dir == KEY_DOWN) target_y = pacman->pos_y + 4;
  
  // Ghost is in chase mode
  if (pink->alive == 1)  ghost_chase(board, target_x, target_y, pink);
  // Ghost is frieghtened
  else if (pink->alive == 0) ghost_frieghtened(board, pink);
  // Ghost is in scatter mode
  else if (pink->alive == 2) ghost_chase(board, 2, 29, pink);
  
  ghost_pacman_interaction(board, pink);
}

// Calculates Cyan Ghost target tile
void cyan_ghost_mover(board_spot **board) {
  // The Cyan Ghost attempts to keep Pacman between himself and the Red Ghost
  // Calculating Cyan target tile
  int vec_x = pacman->pos_x - red->pos_x; 
  int vec_y = pacman->pos_y - red->pos_y;
  if (pacman->dir == KEY_RIGHT) vec_x += 2;
  if (pacman->dir == KEY_LEFT)  vec_x -= 2;
  if (pacman->dir == KEY_UP) vec_y -= 2;
  if (pacman->dir == KEY_DOWN) vec_y += 2; 
  int target_x = vec_x * 2;
  int target_y = vec_y * 2;
  
  // Ghost is in chase mode
  if (cyan->alive == 1) ghost_chase(board, target_x, target_y, cyan);
  // Ghost is frieghtened
  else if (cyan->alive == 0) ghost_frieghtened(board, cyan);
  // Ghost is in scatter mode
  else if (cyan->alive == 2) ghost_chase(board, 27, 29, cyan); 
  
  ghost_pacman_interaction(board, cyan);
}

// Earn extra points for multiple ghosts being eaten in one powerup
void points_eaten_ghost() {
  int ghost_score;
  ghost_score += 200 * pow(2, ghost_eaten - 1);
  score += ghost_score; 
  one_up+= ghost_score;
}

// Controls when Ghosts go into scatter mode
void ghost_scatter(int elapsed_time) {
  // Red scatter
  if ((elapsed_time - num_time_red_scatter * 20 == 20) && red->alive != 0) {
    red->alive = 2;
    num_time_red_scatter ++;
  }
  if ((elapsed_time - (num_time_red_scatter - 1) * 25 == 25) && red->alive != 0) red->alive = 1;
  
  // Pink scatter
  if ((elapsed_time - num_time_pink_scatter * 33 == 33) && pink->alive != 0) {
    pink->alive = 2;
    num_time_pink_scatter ++;
  }
  if ((elapsed_time - (num_time_pink_scatter - 1) * 38 == 38) && pink->alive != 0) pink->alive = 1;

  // Cyan scatter
  if ((elapsed_time - num_time_cyan_scatter * 49 == 49) && cyan->alive != 0) {
    cyan->alive = 2;
    num_time_cyan_scatter ++;
  }
  if ((elapsed_time - (num_time_cyan_scatter - 1) * 54 == 54) && cyan->alive != 0) cyan->alive = 1;

  // Orange scatter
  if ((elapsed_time - num_time_orange_scatter * 57 == 57) && orange->alive != 0) {
    orange->alive = 2;
    num_time_orange_scatter ++;
  }
  if ((elapsed_time - (num_time_orange_scatter - 1) * 62 == 62) && orange->alive != 0) orange->alive = 1;
  
}

// Initializes ghosts to desired states
void ghost_initializer() {
  red->alive = 1; red->out_of_box = 1; red->dir_x = 0; red->dir_y = 0; red->frame_when_dead = 0;
  pink->alive = 1; pink->dir_x = 0; pink->dir_y = 0;  pink->frame_when_dead = 0;
  cyan->alive = 1; cyan->dir_x = 0; cyan->dir_y = 0;  cyan->frame_when_dead = 0;
  orange->alive = 1; orange->dir_x = 0; orange->dir_y = 0; orange->frame_when_dead = 0;
}

void ghost_respawn(ghost* cur_ghost, int frame) {
  bool wait = (frame == cur_ghost->frame_when_dead + (30 / ghost_speed));
  if (wait && (cur_ghost->alive == 0)) {
    cur_ghost->alive = 1;
    cur_ghost->pos_x = 13;
    cur_ghost->pos_y = 11;
    cur_ghost->out_of_box = 1;
    cur_ghost->frame_when_dead = 0;
  }
}

// Resets global variables to desired states
void reset_board(board_spot **board) {
sleep(1);
  
// Reset Fruit number to 0
fruit.on_board = 0;
// Reset pacman
pacman->pos_y = 23;
pacman->pos_x = 14;

// Reset Red Ghost
red->pos_y = 11;
red->pos_x = 13;
red->out_of_box = 1;
red->alive = 1;
red->dir_x = 0;
red->dir_y = 0;
num_time_red_scatter = 0;

// Reset Pink Ghost
pink->pos_y = 14;
pink->pos_x = 12;
pink->out_of_box = 0;
pink->alive = 1;
pink->dir_x = 0;
pink->dir_y = 0;
num_time_pink_scatter = 0;

// Reset Orange Ghost
orange->pos_y = 14;
orange->pos_x = 14;
orange->out_of_box = 0;
orange->alive = 1;
orange->dir_x = 0;
orange->dir_y = 0;
num_time_orange_scatter = 0;
  
// Reset Cyan Ghost;
cyan->pos_y = 14;
cyan->pos_x = 16;
cyan->out_of_box = 0;
cyan->alive = 1;
cyan->dir_x = 0;
cyan->dir_y = 0;
num_time_cyan_scatter = 0;
}

int main(void) {

  // Allocate board
  board_spot **board;
  board = (board_spot **)malloc(BOARD_SIZE_i * sizeof(board_spot *));
  for (int i = 0; i < BOARD_SIZE_i; i++) {
    board[i] = (board_spot *)malloc(BOARD_SIZE_j * sizeof(board_spot));
  }

  // Allocate characters
  pacman = (Pacman *)malloc(sizeof(Pacman));
  red = (ghost *)malloc(sizeof(ghost));
  pink = (ghost *)malloc(sizeof(ghost));
  cyan = (ghost *)malloc(sizeof(ghost));
  orange = (ghost *)malloc(sizeof(ghost));

  // Create Neccessary Timers
  struct timespec t = {0, LOOP_DELAY_NANO};
  struct timeval curr_time;
  struct timeval pac_start;
  struct timeval fruit_timer;
  struct timeval cyan_spawn;
  struct timeval orange_spawn;
  struct timeval pink_spawn;

  // Initialize Pacman and ghosts
  pacman->alive = 1;
  red->alive = 1; red->pos_start_x = 13; red->pos_start_y = 13;
  pink->out_of_box=0; pink->pos_start_x = 12; pink->pos_start_y = 14;
  cyan->out_of_box=0; cyan->pos_start_x = 14; cyan->pos_start_y = 14;
  orange->out_of_box=0; orange->pos_start_x = 16; orange->pos_start_y = 14;
  ghost_initializer();
  
  pacman->dir = 0;
  set_up_print();
  populate_board(board);
  print_board(board);
  
  // Get the time the game first starts
  gettimeofday(&pac_start, NULL);

  // No fruit currently on board
  fruit.on_board = 0;
  fruit.num_fruit_eaten = 0;

  // Starting message
  attron(COLOR_PAIR(7));
  mvprintw(17, 11, "Ready?");
  refresh();
  sleep(3);
  print_highscore();
  
  // MakesPpacman alive and moveable
  while (pacman->alive == 1) {

    gettimeofday(&curr_time, NULL);

    int elapsed_time = (curr_time.tv_sec - pac_start.tv_sec) +
                       (curr_time.tv_usec - pac_start.tv_usec) / 1000000.0;

    // Pink exits box after 10 seconds
    if (elapsed_time == 10 && pink->out_of_box == 0) {
      pink->out_of_box = 1;
      pink->pos_x = 13;
      pink->pos_y = 11;
    }
    else ghost_respawn(pink, frame);
    
    // Cyan exits box after 20 seconds
    if (elapsed_time == 20 && cyan->out_of_box == 0) {
      cyan->out_of_box = 1;
      cyan->pos_x = 13;
      cyan->pos_y = 11;
    }
    else ghost_respawn(cyan, frame);
    
    // Orange exits box after 30 seconds
    if (elapsed_time == 30 && orange->out_of_box == 0) {
      orange->out_of_box = 1;
      orange->pos_x = 13;
      orange->pos_y = 11;
    }
    else ghost_respawn(orange, frame);

    ghost_scatter(elapsed_time);
    ghost_respawn(red, frame);
    
    // Call functions that move all characters
    if (frame % pac_speed == 0) {
      pacman_mover(board);
      ghost_pacman_interaction(board, red);
      ghost_pacman_interaction(board, cyan);
      ghost_pacman_interaction(board, pink);
      ghost_pacman_interaction(board, orange);
    }
    
    if ((pacman->alive == 1) && (frame % ghost_speed == 0)) {
       if (red->out_of_box == 1) red_ghost_mover(board);
       if (cyan->out_of_box == 1) cyan_ghost_mover(board);
       if (pink->out_of_box == 1) pink_ghost_mover(board);
       if (orange->out_of_box == 1) orange_ghost_mover(board);
    }

    // Control how long power up last
    int elapsed_power_up_time = 
        (curr_time.tv_sec - powerup_timer.tv_sec) +
        (curr_time.tv_usec - powerup_timer.tv_usec) / 1000000.0;
    if (elapsed_power_up_time == 10) {
      ghost_initializer();
      pacman->poweredup = 0;
      ghost_eaten = 0;
    }

    // Controlling Fruit
    if ((num_dots_eaten == 70) || (num_dots_eaten == 170)) {
      fruit.on_board = 1;
      gettimeofday(&fruit_timer, NULL);
    }
    int elapsed_fruit_time =
        (curr_time.tv_sec - fruit_timer.tv_sec) +
        (curr_time.tv_usec - fruit_timer.tv_usec) / 1000000.0;
    if (elapsed_fruit_time == 10)
      fruit.on_board = 0;

    frame ++;
    
    // Progressing to next level
    if (num_dots == 0) {
      sleep(1);
      num_dots_eaten = 0;
      num_level += 1;
      if (pac_speed != 3) {
        pac_speed += 1;
        ghost_speed += 1;
        }
      reset_board(board);
      populate_board(board);
      pacman->alive = 1;
      // Reset time of level start
      gettimeofday(&pac_start, NULL);
      frame = 0;
    }

    erase();
    print_hud();
    print_board(board);
    refresh();
    //nanosleep(&t, NULL);
    usleep(125000 / pac_speed);
    
    // Resetting Pacman and ghosts on death
    if (pacman->alive == 2) {
      reset_board(board);
      pacman->alive = 1;
      // Reset time of level start
      gettimeofday(&pac_start, NULL);
      frame = 0;
    }
  }
  sleep(1);
  
  // Game over and highscore update
  attron(COLOR_PAIR(1));
  if(highscore>score) {
    mvprintw(11, 12, "Game");
    mvprintw(17, 12, "Over");
    mvprintw(9, 29, "Zambreno would be dissapointed!");
    attron(COLOR_PAIR(7));
    refresh();
  }
  else {
    mvprintw(11, 12, "Game");
    mvprintw(17, 12, "Over");
    attron(COLOR_PAIR(7));
    mvprintw(9, 29, "You beat the highscore!");
    refresh();
  }
  game_status=0;
  sleep(3);
  endwin();
  print_highscore();
  
  return 0;
}