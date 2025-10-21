这是一个基于raylib和raygui开发的汉诺塔双人对战游戏。玩家可以选择单人或者通过ip连接与其他玩家进行对战，看谁能更快地完成汉诺塔的移动。
什么？你不认识汉诺塔？[点我了解](https://baike.baidu.com/item/%E6%B1%89%E8%AF%BA%E5%A1%94/3468295 "汉诺塔")

## 游戏特点

- 支持联机双人对战
- 实时显示对手的移动情况
- 计时器和计步器功能
- 可自定义汉诺塔层数(1-9)
- 简陋的图形界面

## 游戏玩法

1. 选择单人或者多人
2. 更改游戏配置或使用默认配置
3. 游戏开始时，所有圆盘都在最左边的柱子上
4. 目标是将所有圆盘移动到中间或右边的柱子上
5. 移动规则：
   - 每次只能移动一个圆盘
   - 大圆盘不能放在小圆盘上面
6. 先将所有圆盘移动到目标柱子的玩家获胜

## 按键说明

 1. 使用小键盘的数字键`1` `2` `3`来选择和移动塔
 2. 游戏结束后，按R键再开一局，多人模式下仅服务端能重开游戏
 3. 任何时候按`ESC`或叉关闭游戏

## 编译说明

### 依赖库
- [raylib (v5.0+)](https://github.com/raysan5/raylib/releases)
- [raygui](https://github.com/raysan5/raygui)

### 编译步骤

1. 下载并安装raylib和raygui
2. 修改CMakeLists.txt中的库路径:
```cmake
set(RAYLIB_INCLUDE_DIR 你的raylib包含目录路径)
set(RAYGUI_INCLUDE_DIR 你的raygui源码目录路径)
set(RAYLIB_LIBRARY 你的raylib库文件路径)
```

3. 创建构建目录并编译:
```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```
