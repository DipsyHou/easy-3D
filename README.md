# Easy 3D Engine

基于 SDL2 的像素渲染 3D 引擎（光线投射）。

## 特性

- ✅ 实时像素渲染
- ✅ 光线投射算法
- ✅ 碰撞检测
- ✅ 平滑移动和旋转
- ✅ 距离雾效果
- ✅ 墙体光照计算
- ✅ 支持圆弧墙体

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

## 操作

- **W / ↑** - 前进
- **S / ↓** - 后退
- **A / ←** - 左转
- **D / →** - 右转
- **ESC** - 退出

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

## 与 easy-console3 的区别

| 特性 | easy-console3 | easy-3D |
|------|--------------|---------|
| 渲染方式 | 终端字符 | SDL2 像素 |
| 分辨率 | 80x24 (典型) | 800x600 (可调) |
| 颜色 | 单色字符 | RGB 全彩 |
| 性能 | ~30 FPS | ~60 FPS |
| 平台 | 终端 | 图形窗口 |

## 扩展建议

- [ ] 添加纹理贴图
- [ ] 添加天空盒
- [ ] 添加动态光照
- [ ] 添加多层场景
- [ ] 添加小地图
- [ ] 添加场景编辑器
- [ ] 添加保存/加载功能

## 许可证

MIT License
