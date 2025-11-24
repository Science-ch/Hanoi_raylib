#ifndef GRAPHICS_H
#define GRAPHICS_H

void init_tower(int layer);
bool move_block(int a, int b, int tower);
void handle_input();
void print_tower();
void game_start();
void wait_connection();

#endif // GRAPHICS_H