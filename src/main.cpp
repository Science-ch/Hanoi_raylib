#include <iostream>
#include <string.h>
#include <thread>
#include "raylib.h"
#include "graphics.h"
#include "network.h"
#include "gamestate.h"

using namespace std;
int main(void)
{
	close_cmd();
	game_start();
    if(!is_multiplayer)
	{
		init_tower(layer);
		SetWindowSize(640, 800);
		SetTargetFPS(60);
		game_running = 1;
        thread win_thread(win);
        win_thread.detach();
        while (!WindowShouldClose())
        {
            handle_input();
            print_tower();	
        }
        CloseWindow();
		return 0; 
	}
	else if(is_multiplayer)
    {	
		if(is_server==0)
		{
			init_tower(layer);
			SetWindowSize(1280, 800);
			SetTargetFPS(60);
			game_running = 1;
			thread win_thread(win);
			win_thread.detach();
			while (!WindowShouldClose())
			{
				handle_input();
				print_tower();
			}
			CloseWindow();
			return 0; 
		}
		else if(is_server==1)
		{	
			init_tower(layer);
			thread server_process(server); 			
			wait_connection();
			SetWindowSize(1280, 800);
			SetTargetFPS(60);
			game_running = 1;
            thread win_thread(win);
            win_thread.detach();
			while (!WindowShouldClose())
            {
                handle_input();
                print_tower();
            }
			CloseWindow();
			return 0; 
		}
		else if(is_server==2)
		{
			thread client_process(client);
			wait_connection();
			SetWindowSize(1280, 800);
			SetTargetFPS(60);
			init_tower(layer);
			game_running = 1;
            thread win_thread(win);
            win_thread.detach();
			while (!WindowShouldClose())
            {
                handle_input();
                print_tower();
				if (game_running == 0)
				{
					while(!game_restart)
					{
						print_tower();
						if (WindowShouldClose())
						{
							CloseWindow();
							exit(0);
						}
					}
					game_restart = 0;
					init_tower(layer);
					game_running = 1;
					time_mark2 = time_mark;
				}
				
            }
			CloseWindow();
			return 0; 
		}
		return 0;
	}	
    return 0;
}