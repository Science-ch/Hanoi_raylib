#include "graphics.h"
#include "raylib.h"
#include "gamestate.h"
#include <string.h>
#include <iostream>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void validate_ipv4(char str[16]);

// 初始化塔 
void init_tower(int layer)
{
    if (layer < 1 || layer > 9) layer = 9;
    
    selected_tower = -1;
    selected_tower2 = -1;
    game_won = 0;
    selection_state = 0;
    game_running = 0;
    game_restart = 0;
    self[0] = 0;
    self[1] = 0;
    other[0] = 0;
    other[1] = 0;
    count = 0;
    count2 = 0;

    self_tower[0].clear();
    self_tower[1].clear();
    self_tower[2].clear();
    other_tower[0].clear();
    other_tower[1].clear();
    other_tower[2].clear();

    for(int j=layer;j>0;j--)
    {
        self_tower[0].push_front(j);
        other_tower[0].push_front(j);
    }
}

// 移动方块
bool move_block(int a,int b)
{
    if(a<0||a>2||b<0||b>2) return false;
    else if(self_tower[a].empty()) return false;
    else if(self_tower[b].empty())
    {
        self_tower[b].push_front(self_tower[a].front());
        self_tower[a].pop_front();
        if(b>0)self[b==1?0:1]=self_tower[b].size();
        return true;
    }
    else if(self_tower[a].front()<self_tower[b].front())
    {
        self_tower[b].push_front(self_tower[a].front());
        self_tower[a].pop_front();
        if(b>0)self[b==1?0:1]=self_tower[b].size();
        return true;		
    }
    else
    return false;
}

// 绘制塔
void print_tower()
{
    char str[20];
    BeginDrawing();
    ClearBackground(RAYWHITE); // 白色背景
    
    // 绘制自己的塔
    for (int i = 0; i < 3; i++) 
    {
        // 绘制底座和柱子
        DrawRectangle(15 + i * 200, 450, 150, 20, BROWN);
        DrawRectangle(85 + i * 200, 100, 10, 350, BROWN);
        
        // 绘制盘子
        int n = 0;

        for(auto j=self_tower[i].rbegin(); j!=self_tower[i].rend(); ++j) 
        {
            int block = *j; // 获取盘子大小
            DrawRectangleRounded(
                Rectangle{(float)(70 - block * 5 + i * 200), (float)(420 - n * 30), 
                         (float)(40 + block * 10), 30.0}, 
                0.5f, 10, SKYBLUE);
            
            // 绘制盘子上的数字
            char num_str[2] = { (char)('0' + block), '\0' };
            DrawText(num_str, 85 + i * 200, 425 - n * 30, 20, DARKBLUE);
            n++;
        }
    }
    
    if (is_multiplayer)
    {
        for (int i = 0; i < 3; i++) 
        {
            // 绘制底座和柱子（位置+700）
            DrawRectangle(15 + i * 200 + 700, 450, 150, 20, BROWN);
            DrawRectangle(85 + i * 200 + 700, 100, 10, 350, BROWN);
            
            // 绘制盘子
            int n = 0;

            for(auto j=other_tower[i].rbegin(); j!=other_tower[i].rend(); ++j) 
            {
                int block = *j; // 获取盘子大小
                DrawRectangleRounded(
                    Rectangle{(float)(70 - block * 5 + i * 200 + 700), (float)(420 - n * 30), 
                             (float)(40 + block * 10), 30.0}, 
                    0.5f, 10, GREEN);
                
                // 绘制盘子上的数字
                char num_str[2] = { (char)('0' + block), '\0' };
                DrawText(num_str, 85 + i * 200 + 700, 425 - n * 30, 20, DARKGREEN);
                n++;
            }
            sprintf( str, "Step:%4d", count2);
            DrawText( str, 230 + 700, 680, 32, GRAY);
        }
    }
    
    // 绘制选中的塔的标记
    if (selected_tower > -1 && game_running) 
    {
        if(!first_press)
        {
            first_press=1;
            time_mark = GetTime();
        }
        DrawRectangleLines(15 + selected_tower * 200, 100, 150, 350, RED);
    }
    if (selected_tower2 > -1 && is_multiplayer && game_running) 
    {
        if(!first_press)
        {
            first_press=1;
            time_mark = GetTime();
        }
        DrawRectangleLines(715 + selected_tower2 * 200, 100, 150, 350, RED);
    }
    
    // 绘制胜利文本
    if (game_won)
     {
        DrawText("Finish!", 230 + 700 * (game_won == 1 ? 0 : 1), 600, 40, PURPLE);
    }

    // 绘制计步器和计时器
    sprintf( str, "Step:%4d", count);
    DrawText( str, 230, 680, 32, GRAY);
    if (game_running && first_press)
        time_mark2 = GetTime();
    sprintf( str, "Time:%.2fs", time_mark2 - time_mark);
    DrawText( str, 10, 750, 32, GOLD);
    

    // 根据游戏状态更新窗口标题
    if (game_running == 0)
    {
        if (is_multiplayer)
        {
            if (is_server == 1) 
            {
                if (game_won == 1)
                    SetWindowTitle("Hanoi Game (Server) - You Win! Press R to Restart");
                else if (game_won == 2) 
                    SetWindowTitle("Hanoi Game (Server) - Game Over! Press R to Restart");
            }
            else if (is_server == 2)
            {
                if (game_won == 1)
                    SetWindowTitle("Hanoi Game (Client) - You Win! Wating for Restart...");
                else if (game_won == 2) 
                    SetWindowTitle("Hanoi Game (Client) - You Lost! Wating for Restart...");
            }
        }
        else if (game_won == 1) 
            SetWindowTitle("Hanoi Game - You Win! Press R to Restart");
    }
    else
    {
        if (is_multiplayer)
        {
            if (is_server == 1) 
                SetWindowTitle("Hanoi Game (Server)");
            else if (is_server == 2)
                SetWindowTitle("Hanoi Game (Client)");
        }
        else
            SetWindowTitle("Hanoi Game");
    }

    EndDrawing();
}

// 处理输入
void handle_input() {

    int key = GetKeyPressed();
    
    // 处理数字键1-3
    if (key >= KEY_KP_1 && key <= KEY_KP_3 && game_running) 
    {
        int tower_index = key - KEY_KP_1; // 转换为0-2
        
        if (selection_state == 0) 
        { // 第一次选择
            selected_tower = tower_index;
            selection_state = 1;
        } else { // 第二次选择
            if (selected_tower != tower_index) 
            { // 不能选择同一个塔
                if (move_block(selected_tower, tower_index))
                    count++;
            }
            // 重置选择状态
            selected_tower = -1;
            selection_state = 0;
        }
    }
    if (game_running == 0)
    {
        if (key == KEY_R && is_server == 1) 
        {
            if (is_multiplayer)
            {
                game_restart = 1;
                while (!game_restart_send);
                game_restart_send = 0;
            }
            init_tower(layer);
            game_running = 1;
            time_mark2 = time_mark;
        }    
    }
}


void game_start()
{
    InitWindow(640, 800, "Hanoi Game");
    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(60);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 32);
    int fileSize;
    bool spinnerEditMode = false;
    char textBoxText[16] = "";
    bool textBoxEditMode = false;
    bool valueBoxEditMode = false;
    unsigned char *fontFileData = LoadFileData("c:\\windows\\fonts\\simhei.ttf",&fileSize);
    char text[] = "单人多人设置层数开始游戏地址服务器客户端口号： 0123456789IP";
    int codepointsCount;
    int *codepoints=LoadCodepoints(text,&codepointsCount);
    Font font = LoadFontFromMemory(".ttf",fontFileData,fileSize,32,codepoints,codepointsCount);
    UnloadCodepoints(codepoints);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleRounded(Rectangle{150, 50, 100, 50}, 0.5f, 20, is_multiplayer? LIGHTGRAY : GREEN);
        DrawRectangleRounded(Rectangle{390, 50, 100, 50}, 0.5f, 20, is_multiplayer? GREEN : LIGHTGRAY);
        DrawRectangleRounded(Rectangle{220, 600, 200, 50}, 0.5f, 20, GREEN);
        DrawTextEx(font,"单人",Vector2{168,60},32,0,BLACK);
        DrawTextEx(font,"多人",Vector2{408,60},32,0,BLACK);
        DrawTextEx(font,"开始游戏",Vector2{250,610},32,0,BLACK);
        if (!is_multiplayer)
        {
            DrawTextEx(font,"设置层数：",Vector2{100,150},32,0,BLACK);
            if (GuiSpinner((Rectangle){ 250, 150, 100, 40 }, NULL, &layer, 1, 9, spinnerEditMode)) {
                spinnerEditMode = !spinnerEditMode;
            }
        }
        if (is_multiplayer)
        {
            DrawRectangleRounded(Rectangle{150, 230, 130, 40}, 0.5f, 20, is_server==1? GREEN : LIGHTGRAY);
            DrawRectangleRounded(Rectangle{360, 230, 130, 40}, 0.5f, 20, is_server==2? GREEN : LIGHTGRAY);
            DrawTextEx(font,"服务器",Vector2{165,235},32,0,BLACK);
            DrawTextEx(font,"客户端",Vector2{375,235},32,0,BLACK);
            if (is_server==1)
            {
                DrawTextEx(font,"设置层数：",Vector2{100,150},32,0,BLACK);
                if (GuiSpinner((Rectangle){ 250, 150, 100, 40 }, NULL, &layer, 1, 9, spinnerEditMode)) {
                    spinnerEditMode = !spinnerEditMode;
                }
                DrawTextEx(font,"端口号：",Vector2{100,330},32,0,BLACK);
                if (GuiValueBox((Rectangle){ 230, 330, 130, 40 }, NULL, &port, 1,65535, valueBoxEditMode)) {
                    valueBoxEditMode = !valueBoxEditMode;
                }
            }
            else if(is_server==2)
            {
                DrawTextEx(font,"IP地址：",Vector2{100,330},32,0,BLACK);
                if (GuiTextBox((Rectangle){ 230, 330, 270, 40 }, textBoxText, 16, textBoxEditMode)) {
                    textBoxEditMode = !textBoxEditMode;
                    if(!textBoxEditMode)validate_ipv4(textBoxText);
                }
                DrawTextEx(font,"端口号：",Vector2{100,430},32,0,BLACK);
                if (GuiValueBox((Rectangle){ 230, 430, 130, 40 }, NULL, &port, 1,65535, valueBoxEditMode)) {
                    valueBoxEditMode = !valueBoxEditMode;
                }
            }
            
            
        }
        
        EndDrawing();
        Vector2 mousePoint = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (mousePoint.x >= 150 && mousePoint.x <= 250 && mousePoint.y >= 50 && mousePoint.y <= 100)
                is_multiplayer = 0;
            else if (mousePoint.x >= 390 && mousePoint.x <= 490 && mousePoint.y >= 50 && mousePoint.y <= 100)
                is_multiplayer = 1;
            else if (mousePoint.x >= 220 && mousePoint.x <= 420 && mousePoint.y >= 600 && mousePoint.y <= 650)
            {
                UnloadFont(font);
                UnloadFileData(fontFileData);
                return;
            }
            if (is_multiplayer)
            {
                if (mousePoint.x >= 150 && mousePoint.x <= 280 && mousePoint.y >= 230 && mousePoint.y <= 270)
                    is_server = 1;
                else if (mousePoint.x >= 360 && mousePoint.x <= 490 && mousePoint.y >= 230 && mousePoint.y <= 270)
                    is_server = 2;
            }
            
        }        
    }
    UnloadFont(font);
    UnloadFileData(fontFileData);
    CloseWindow();
    exit(0);
}

void validate_ipv4(char str[16]) 
{
    int num, dots = 0;
    char temp[16];
    strcpy(temp, str);
    int dot_count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            dot_count++;
            if (i == 0 || str[i+1] == '.' || str[i+1] == '\0') {
                strcpy(str, "invalid");
                return;
            }
        }
    }
    
    if (dot_count != 3) {
        strcpy(str, "invalid");
        return;
    }
    
    char *token = strtok(temp, ".");
    int part_count = 0;
    
    while (token != NULL) {
        part_count++;
        
        if (strlen(token) == 0) {
            strcpy(str, "invalid");
            return;
        }
        
        for (int i = 0; token[i] != '\0'; i++) {
            if (!isdigit(token[i])) {
                strcpy(str, "invalid");
                return;
            }
        }
        
        if (strlen(token) > 1 && token[0] == '0') {
            strcpy(str, "invalid");
            return;
        }
        
        num = atoi(token);
        if (num < 0 || num > 255) {
            strcpy(str, "invalid");
            return;
        }
        
        token = strtok(NULL, ".");
    }
    
    if (part_count != 4) {
        strcpy(str, "invalid");
        return;
    }
    
    ip_address = std::string(str);
}

void wait_connection()
{
    int fileSize;
    unsigned char *fontFileData = LoadFileData("c:\\windows\\fonts\\simhei.ttf",&fileSize);
    char text[] = "等待连接. ";
    int codepointsCount;
    int *codepoints=LoadCodepoints(text,&codepointsCount);
    Font font = LoadFontFromMemory(".ttf",fontFileData,fileSize,32,codepoints,codepointsCount);
    UnloadCodepoints(codepoints);
    while (!connected)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextEx(font,"等待连接 ...",Vector2{220,350},32,0,BLACK);
        EndDrawing();
        if(WindowShouldClose()) 
        {
            CloseWindow();
            exit(0);
        }
    }
    UnloadFont(font);
    UnloadFileData(fontFileData);
}
