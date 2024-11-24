# 小彭老师现代 Qt 项目实战

小彭老师带你学习现代 Qt，结合实际项目边学边做，妈妈再也不用担心我的“铁饭碗”。

## 安装开发环境

小彭老师自己用的是：

- Qt 5.15.15
- GCC 14.2.1
- Arch Linux

但是其他平台和编译器应该也能兼容。

> 如有不兼容问题，请通过 [GitHub issues](https://github.com/parallel101/qtguide/issues) 反馈。

### Arch Linux

推荐使用 Linux 系统，一行命令搞定依赖。

```bash
pacman -S qt5-base
```

> 警告：不推荐使用 WSL 或 WSL2，因为它们只有命令行界面，不支持图形界面和显卡，导致之后的 Qt + OpenGL 实战困难。如果你能配出图形界面 + 显卡支持，那你牛币，大多数人是配不明白的。

### Windows

欢迎来到地狱副本。

[点击此处下载 Qt 5.12.12 离线安装包](https://download.qt.io/archive/qt/5.12/5.12.12/qt-opensource-windows-x86-5.12.12.exe)

> **警告**：由于 Qt 公司的**傻逼公关**，如果你是从官网下载的最新版安装包，它会强制给你下载一个所谓的“联网安装包”！在线安装包会强制要求你注册一个它们的**狗屎帐号**才能继续下一步，而注册会强制要求填你的**电子邮件地址、公司名、职务、软件用途**等信息才能继续安装，非常傻逼！
>
> 所以我给出的链接是 Qt 5.12.12 版的离线安装包，这是最后一个支持离线安装的版本！更新的版本中它们只提供“联网安装包”！有能力的同学可以尝试从源码手动构建更新版的 Qt 绕开被联网信息强奸。

安装时会让你勾选 `mingw53_64`，`msvc2017_64` 等选项。

这是 Windows C++ 的多个不同版本的 ABI，互不兼容！

- 如果你想要使用 64 位 MSVC 编译器，那就得勾选 `msvc2017_64` 来安装，安装以后的 dll 文件会位于 `C:/Qt5/msvc2017_64/bin`，需要设置环境变量 `Qt5_DIR` 为 `C:/Qt5/msvc2017_64/lib/cmake`，添加环境变量 `PATH` 为 `C:/Qt5/mingw53_64/bin`。
- 如果你想要使用 64 位 MinGW 编译器，那就得勾选 `mingw53_64` 来安装，安装以后的 dll 文件会位于 `C:/Qt5/mingw53_64/bin`，需要设置环境变量 `Qt5_DIR` 为 `C:/Qt5/mingw53_64/lib/cmake`，添加环境变量 `PATH` 为 `C:/Qt5/mingw53_64/bin`。

建议新手勾选 `mingw53_64`、`Qt Creator`、`MinGW 编译器` 选项，避免配置编译器的烦恼。

> 如果勾选了 `msvc2017_64`，那么你还需要单独再安装一个 VS2022，这货安装起来比 Qt 还慢。

> 如果你的硬盘空间不值钱，那就都勾选吧，反正装多了也不吃亏。

Qt Creator 是 Qt 自带的一款 IDE，具有语义高亮、跳转定义、断点调试等功能，且与 Qt 库集成甚佳。

## 构建实验源码

在本项目根目录中，有许多形如 `01_xxx` 的子文件夹，每一个都是一期课程的源码。

以第一期课程为例：

第一期课程是实现一个“九九乘法表”，源码位于 `01_9x9_table` 子文件夹下。

请先进入此子文件夹后，再运行 CMake 构建。

对于 IDE 用户则请打开此子文件夹下的 CMakeLists.txt。

### Linux / MacOS

进入第一课的源码目录，开始构建：

```bash
cd 01_9x9_table
cmake -B build
cmake --build build
```

构建完成后会生成 `build/01_9x9_table` 这个可执行文件。

### Windows

用你喜欢的 IDE（Qt Creator 或 VS2022）打开 `01_9x9_table` 文件夹下的 `CMakeLists.txt`。

打开后 IDE 会自动配置 CMake，然后按 Ctrl+B 即可构建。

如果 CMake 报错找不到 Qt5，可能是因为你安装在非默认路径，请你在 `find_package(Qt5)` 前加一行：

```cmake
set(Qt5_DIR D:\Qt5\mingw53_64\lib\cmake)
```

> 警告：此处的 `D:\Qt5\mingw53_64` 要改成你的 Qt 安装路径，请替换成你自己真正的安装路径！否则找不到！

后面的 `\lib\cmake` 是必须的后缀，在你的安装路径后面加上 `\lib\cmake` 后缀，才能找到。

> 原理是 CMake 会从这个路径中中寻找 `Qt5Config.cmake` 这个文件，如果你找到了这个文件，请设为这个文件所在的文件夹。

设置后，请你删除整个 `build` 或 `out` 文件夹，然后重新运行 CMake，否则设置无效！

## 运行实验程序

### Linux

运行可执行文件：

```bash
build/01_9x9_table
```

> 如果报错，说明你可能是 WSL 的受害者，请检查你是否真的配置好了图形界面。你可以运行如 `xclock` 这类非 Qt 的图形程序，检查图形界面是否正常，从而排除 Qt 本身的问题。

### Windows

IDE 打开 `01_9x9_table` 文件夹下的 `CMakeLists.txt`。

IDE 中点击运行键或按 F5，即可运行程序。

也可以手动找到 `out\x64-Debug\01_9x9_table.exe` 双击运行。

如果没有配置过 `PATH` 变量，会报错找不到 `Qt5Widgets.dll`，请把 `C:\Qt5\mingw53_64\bin` 添加到 `PATH` 后重启 IDE 即可生效，如果还不行就重启电脑。

> 警告：此处的 `C:\Qt5\mingw53_64` 要改成你的 Qt 安装路径，请替换成你自己真正的安装路径！否则找不到！

如果不想修改 `PATH`，也可以在命令提示符中运行：

```bash
cd out\x64-Debug
C:\Qt5\mingw53_64\bin\windeployqt.exe 01_9x9_table.exe
```

这里的 `windeployqt` 是一个实用工具，参数指定为你的 exe 文件运行一遍后，会自动把所有 Qt 所需的 dll 自动拷贝到和当前可执行文件同目录，从而让你的 exe 运行时能找得到它们。
