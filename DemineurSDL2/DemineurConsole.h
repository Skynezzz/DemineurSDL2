#ifndef DEMINEURCONSOLE
#define DEMINEURCONSOLE

#define GRID_SIZE_C 10
#define BOMB_COUNT_C 17

#define GRID_HIDE_CASE_C 0
#define GRID_SHOW_CASE_C 1
#define GRID_BOMB_CASE_C 2

void displayGrid_C(int grid[GRID_SIZE_C][GRID_SIZE_C], int loose);
int setBomb_C(int grid[GRID_SIZE_C][GRID_SIZE_C], int startPosX, int startPosY);
int bombArround_C(int grid[GRID_SIZE_C][GRID_SIZE_C], int posX, int posY);
int findSafeCase_C(int grid[GRID_SIZE_C][GRID_SIZE_C], int posX, int posY);
int isVictory_C(int grid[GRID_SIZE_C][GRID_SIZE_C]);

#endif