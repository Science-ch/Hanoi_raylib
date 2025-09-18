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
    cout<<"1.���� 2.����"<<endl;
    cin>>choose;
    if(choose==1)
	{
		is_multiplayer = 0;
		cout<<"�������(1~9):";
		cin>>layer;
        if (layer < 1 || layer > 9) layer = 3;
		init_tower(layer);
		InitWindow(640, 800, "Hanoi Game");
		SetTargetFPS(60);
        close_cmd();
		game_running = 1;
        thread win_thread(win);
        win_thread.detach();
		time_mark = GetTime();
        while (!WindowShouldClose())
        {
            handle_input();
            print_tower();	
        }
        CloseWindow();
		return 0; 
	}
	else if(choose==2)
    {
        is_multiplayer = 1;
		cout<<"1.���������� 2.ͨ��ip����"<<endl;
		cin>>choose;
		if(choose==1)
		{
			cout<<"ѡ��˿�:";
			cin>>port;
			cout<<"�������:";
			cin>>layer;
            if (layer < 1 || layer > 9) layer = 3;	
			init_tower(layer);
			thread server_process(server); 			
			while(!connected);
			InitWindow(1280, 800, "Hanoi Game (Server)");
			SetTargetFPS(60);
            close_cmd();
			game_running = 1;
            thread win_thread(win);
            win_thread.detach();
			time_mark = GetTime();
			while (!WindowShouldClose())
            {
                handle_input();
                print_tower();
            }
			return 0; 
		}
		else if(choose==2)
		{
			cout<<"����ip:";
			cin>>ip_address;
			cout<<"����˿�:";
			cin>>port;
			thread client_process(client);
			while(!connected);
			InitWindow(1280, 800, "Hanoi Game (Client)");
			SetTargetFPS(60);
			init_tower(layer);
            close_cmd();
			game_running = 1;
            thread win_thread(win);
            win_thread.detach();
			time_mark = GetTime();
			while (!WindowShouldClose())
            {
                handle_input();
                print_tower();
				if (game_running == 0)
				{
					while(!game_restart)
					{
						print_tower();
						if (WindowShouldClose()) return 0;
					}
					game_restart = 0;
					init_tower(layer);
					game_running = 1;
					time_mark = GetTime();
				}
				
            }
			return 0; 
		}
		return 0;
	}	
    return 0;
}