# Easy2.5 Engine

基于 SDL2 的像素渲染 3D 引擎（光线投射）。

## 依赖

- C++17 编译器
- SDL2 开发库

### 安装 SDL2

**Ubuntu/Debian:**
```bash
sudo apt-get install libsdl2-dev
```

**macOS:**
```bash
brew install sdl2
```

**Windows (WSL):**
```bash
sudo apt-get install libsdl2-dev
```

## 编译运行

```bash
# 编译
make

# 运行
./easy3d

# 或一键编译运行
make run
```

## 项目结构

```
easy-3D/
├── viewpoint.hpp  - 视角管理
├── wall.hpp       - 墙体定义
├── space.hpp      - 场景渲染
├── main.cpp       - 主程序
├── Makefile       - 构建脚本
└── README.md      - 说明文档
```
