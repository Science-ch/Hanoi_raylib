#include "gamestate.h"

std::list<int> self_tower[3];             // 自己的塔
std::list<int> other_tower[3];            // 对方的塔
int layer = 3;                            // 层数
int selected_tower = -1;                  // 当前选择的塔
int selected_tower2 = -1;                 // 对方选择的塔
int self[2];                              // 自己的状态
int other[2];                             // 对方的状态
int port = 7777;                          // 端口号
std::string ip_address;                   // IP地址
bool connected;                           // 是否已连接
bool is_multiplayer = 0;                  // 是否为多人游戏
int game_won = 0;                         // 游戏是否胜利
bool game_running = 0;                    // 游戏是否运行
bool game_restart = 0;                    // 游戏是否重启
bool game_restart_send = 0;               // 游戏重启信号已发送
int is_server = 1;                        // 是否为服务端
int count = 0;                            // 计步器
int count2 = 0;                           // 对方计步器
double time_count = 0;                    // 计时器
double time_mark = 0;                     // 计时标记
double time_mark2 = 0;                    // 计时标记2
bool first_press = 0;                     // 是否首次按键
int selection_state = 0;                  // 选择状态
std::mutex mtx;   