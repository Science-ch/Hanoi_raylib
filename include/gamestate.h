#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <list>
#include <string>
#include <vector>
#include <mutex>

extern std::list<int> self_tower[3];             // 自己的塔
extern std::list<int> other_tower[3];            // 对方的塔
extern int layer;                                // 层数
extern int selected_tower;                       // 当前选择的塔
extern int selected_tower2;                      // 对方选择的塔
extern int port;                                 // 端口号
extern std::string ip_address;                   // IP地址
extern int self[2];                              // 自己的状态
extern int other[2];                             // 对方的状态
extern bool connected;                           // 是否已连接
extern bool is_multiplayer;                      // 是否为多人游戏
extern int game_won;                             // 游戏是否胜利
extern bool game_running;                        // 游戏是否运行
extern bool game_restart;                        // 游戏是否重启
extern bool game_restart_send;                   // 游戏重启信号已发送
extern int is_server;                            // 是否为服务端
extern int selection_state;                      // 选择状态
extern int count;                                // 计步器
extern int count2;                               // 对方计步器
extern double time_count;                        // 计时器
extern double time_mark;                         // 计时标记
extern double time_mark2;                        // 计时标记2
extern bool first_press;                         // 是否首次按键
extern std::mutex mtx;                           // 互斥锁，保护共享数据


#endif // GAMESTATE_H