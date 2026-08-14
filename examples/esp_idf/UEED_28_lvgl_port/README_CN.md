# UEED32480035E-WB-A LVGL 示例

[English](./README.md)

本示例用于 **UEED32480035E-WB-A** 3.5 英寸 320×480 半透半反触摸屏，基于 **ESP-IDF + LVGL**。烧录成功后，屏幕会显示操作说明。可用板载 **BOOT** 键或触摸屏进入轮播、切图、开关背光。

推荐开发环境：**VS Code + 乐鑫 ESP-IDF 插件**，ESP-IDF 版本 **v5.1 / v5.2 / v5.3**（本工程按 **v5.3** 编译验证）。

本文按初学者从零开始的顺序编写。请尽量按步骤操作，不要跳过「打开的必须是示例文件夹」这一条。

## 准备材料

- UEED32480035E-WB-A 开发板
- USB Type-C 数据线（必须能传数据，不要用“仅充电”线）
- 5 V / 1 A 电源适配器，或供电充足的电脑 USB 口
- 已联网的电脑（Windows / macOS / Linux）。**第一次编译**会从网络下载组件

## 硬件连接

1. 将屏幕排线插入板载屏幕座，**金色触点朝上**，插到位。
2. 用 Type-C 连接开发板和电脑，电源指示灯应亮起。
3. **第一次烧录**，或一直提示下载失败时：按住 **BOOT** 不放 → 按一下 **RESET** → 松开 **BOOT**。芯片会停在下载模式，直到开始烧录。

本板 USB 转串口芯片是 **CH340C**。Windows 上若插上后没有出现新的 COM 口，请先安装 [CH340 驱动](https://www.wch.cn/downloads/CH341SER_EXE.html)，再拔插 USB，打开「设备管理器 → 端口 (COM 和 LPT)」确认端口号。

---

## 第一步：安装 VS Code

1. 打开 [Visual Studio Code 官网](https://code.visualstudio.com/) 下载并安装。
2. 同时安装 [Git](https://git-scm.com/downloads)。ESP-IDF 安装向导需要 Git。  
   Windows 安装 Git 时，一路 Next 即可。

装好后能打开 VS Code，并且在命令行里输入 `git --version` 能显示版本号，即可进入下一步。

## 第二步：安装 ESP-IDF 插件

1. 打开 VS Code。
2. 点击左侧 **扩展** 图标（四个方块），或按 `Ctrl+Shift+X`（macOS 为 `Cmd+Shift+X`）。
3. 搜索 **ESP-IDF**。
4. 安装发布者为 **Espressif Systems** 的 **ESP-IDF** 扩展。

官方说明：[VS Code ESP-IDF 插件文档](https://docs.espressif.com/projects/vscode-esp-idf-extension/zh_CN/latest/)。

## 第三步：用乐鑫 EIM 一键安装 ESP-IDF 工具链

插件只是界面，真正编译还需要 ESP-IDF SDK 和编译器。请使用乐鑫官方的 **ESP-IDF Installation Manager（EIM）** 图形界面一键安装，不要再在 VS Code 里手动配路径。

官方说明：[EIM 文档](https://docs.espressif.com/projects/idf-im-ui/en/latest/) · [Windows 安装指南](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32s3/get-started/windows-setup.html)

### 3.1 下载并安装 EIM

打开 [EIM 下载页](https://dl.espressif.com/dl/eim/)，下载 **GUI（图形界面）在线安装包**。

也可以按系统安装：

| 系统 | 推荐方式 |
| :--- | :--- |
| Windows | 双击下载的 `.exe`；或在终端执行 `winget install Espressif.EIM` |
| macOS | `brew tap espressif/eim` 然后 `brew install --cask eim-gui` |
| Linux | 见下载页的 apt / Homebrew 说明 |

安装完成后，从开始菜单或应用程序里打开 **ESP-IDF Installation Manager**。

### 3.2 一键安装 ESP-IDF（简易安装）

保持网络畅通，然后按界面操作：

1. 在 **新安装** 下点击 **开始安装**。  
   （如果电脑上还没有装过 ESP-IDF，界面里可能只有「新安装」，没有「管理安装」。）
2. 在 **简易安装** 下点击 **开始简易安装**。  
   这会按默认设置安装当前最新稳定版 ESP-IDF 及全部工具链，适合初学者。
3. 先决条件和路径检查通过后，进入 **准备安装** 页面，再点 **开始安装**。
4. 等待进度走完，出现 **安装完成** 即可。一般需要 10～30 分钟。

> 本示例需要 ESP-IDF **v5.1 / v5.2 / v5.3**（避开 v5.1.1）。简易安装装的是最新稳定版，通常可以直接用。  
> 若必须指定 **v5.3.x**，请改用界面里的 **自定义安装**，选择对应版本；安装路径不要包含中文或空格。

安装失败时：点界面底部 **日志** 查看原因，修好后点 **重试**。

### 3.3 让 VS Code 识别刚装好的 IDF

EIM 装好的版本会被 ESP-IDF 插件自动发现。回到 VS Code：

1. 按 `F1` 或 `Ctrl+Shift+P` 打开命令面板。
2. 运行 **`ESP-IDF: Select Current ESP-IDF Version`**，选中 EIM 刚安装的版本。
3. （建议）再运行 **`ESP-IDF: Doctor Command`**。报告没有明显红色错误，说明环境可用。

> 工程路径同样建议只用英文，例如 `D:\github\UEED32480035...`。路径里有中文时，部分工具可能报错。

## 第四步：打开本示例文件夹

> **非常重要：打开的必须是本示例目录，不是仓库根目录。**

1. VS Code 菜单：**文件 → 打开文件夹...**（File → Open Folder...）
2. 选中下面这个目录：

```text
<仓库根目录>/examples/esp_idf/UEED_28_lvgl_port
```

判断有没有开对：这个文件夹里能直接看到 `CMakeLists.txt`、`sdkconfig`、`main`。  
如果打开的是仓库根目录（里面是 `README.md`、`datasheet`、`examples`），底部的编译/烧录按钮会找不到工程。

打开后稍等几秒，等 ESP-IDF 插件激活。窗口**最底部状态栏**一般会出现：当前芯片、COM 口、编译、烧录、监视等按钮。

## 第五步：确认屏幕型号宏

用 VS Code 打开 `main/board.h`，确认已选择 **3.5 英寸半透半反屏**（仓库默认就是这个）：

```c
#define VIEWE_24_T      0
#define VIEWE_28_T      0
#define VIEWE_35_T      1   // 3.5 英寸半透半反 320x480
```

三个宏只能有一个为 `1`。改完文件后 `Ctrl+S` 保存。

## 第六步：设置芯片目标为 ESP32-S3

1. 命令面板 → **`ESP-IDF: Set Espressif Device Target`**
2. 选择 **`esp32s3`**。
3. 若继续询问开发板型号，选通用的 ESP32-S3 即可（例如 **ESP32-S3 chip (via ESP-PROG)**）。本产品不是乐鑫官方 DevKit，不必选 DevKitC。

状态栏应显示 `esp32s3`。目标设错（例如还是 esp32）会导致编译或启动失败。

## 第七步：检查 sdkconfig（建议）

命令面板 → **`ESP-IDF: SDK Configuration Editor`**（就是图形化的 `menuconfig`）。

请核对这些项（仓库里的 `sdkconfig` 大部分已配置，但 **Flash 大小务必为 16 MB**）：

| 配置路径 | 建议值 | 说明 |
| :--- | :--- | :--- |
| Serial flasher config → Flash SPI mode | DIO | 与本模组匹配 |
| Serial flasher config → Flash size | **16 MB** | 分区表里应用分区是 12 MB |
| Component config → ESP PSRAM | 打开外部 SPI RAM | 模组带 8 MB 八线 PSRAM |
| SPI RAM → Mode | **Octal / OPI** | ESP32-S3-N16R8 |
| ESP System Settings → CPU frequency | **240 MHz** | 默认性能 |

本示例 `partitions.csv` 中 `factory` 应用分区为 **12 MB**，因此 Flash **必须选 16 MB**。若仍是 8 MB，编译或烧录会失败。

点击 **Save** 保存后关闭。第一次打开配置界面可能较慢，属正常现象。

## 第八步：选择串口和烧录方式

1. 用 USB 线连接开发板。
2. 命令面板 → **`ESP-IDF: Select Port to Use (COM, tty, usbserial)`**
   - Windows：`COMx`（CH340）
   - macOS：`/dev/cu.usbserial-xxxx`
   - Linux：`/dev/ttyUSB0` 等
3. 命令面板 → **`ESP-IDF: Select Flash Method`** → 选 **UART**

选中的端口会出现在底部状态栏。插上板子却看不到端口时，回到上文安装 CH340 驱动。

Linux 若没有权限访问串口，可执行 `sudo usermod -a -G dialout $USER`，然后重新登录。

## 第九步：编译

任选一种方式：

- 点击底部状态栏的 **Build（编译）** 按钮；或
- 命令面板 → **`ESP-IDF: Build Your Project`**

**第一次编译**会通过组件管理器下载 LVGL、`esp_lvgl_port` 等依赖到 `managed_components/` 目录，需要联网，可能要几分钟到十几分钟。

成功时终端末尾类似：

```text
Project build complete. To flash, run:
idf.py flash
```

若编译失败：

1. 确认打开的是 `UEED_28_lvgl_port`，不是仓库根目录。
2. 确认目标芯片是 `esp32s3`。
3. 命令面板 → **`ESP-IDF: Full Clean Project`**（彻底清理）后再编译一次。
4. 若卡在下载组件，检查网络，或为 ESP 组件仓库配置国内镜像。

## 第十步：烧录并查看日志

推荐一次完成编译、烧录、监视：

1. 按住 **BOOT** → 按一下 **RESET** → 松开 **BOOT**（首次或失败时必做）。
2. 命令面板 → **`ESP-IDF: Build, Flash and Start a Monitor on Your Device`**
3. 若弹出烧录方式，选 **UART**。

也可以分三步：先 **Build**，再 **Flash**，最后 **Monitor**。

烧录成功后芯片会复位，监视窗口开始刷日志，屏幕出现英文操作说明。

退出串口监视：在监视终端按 `` Ctrl+] ``。

> 监视占用 COM 口。若要重新烧录，先关掉监视，或直接再用「编译+烧录+监视」命令（它会先结束旧监视）。

## 上电后怎么操作

**BOOT 按键**和**触摸屏**手势相同：

| 操作 | 效果 |
| :--- | :--- |
| 双击 / 双指点击 | 进入图片轮播 |
| 单击（轮播界面） | 切换下一张图 |
| 长按 | 打开 / 关闭背光 |

轮播图为 `EV_35_1` / `EV_35_2` / `EV_35_3`，分辨率 320×480。自行换图时不要超过该分辨率。

半透半反屏在户外可借助环境光；室内或夜间请打开背光（GPIO13，长按可切换）。

## 工程结构（便于对照代码）

```text
UEED_28_lvgl_port/
├── CMakeLists.txt          ESP-IDF 工程入口
├── sdkconfig               当前编译配置
├── partitions.csv          分区表（应用分区 12 MB）
├── main/
│   ├── main.c              LVGL 界面、按键与触摸逻辑
│   ├── board.c / board.h   屏幕 / 触摸初始化与引脚
│   ├── EV_35_*.c           320×480 轮播图片
│   └── idf_component.yml   LVGL、LCD 驱动版本
└── components/             本地触摸驱动
```

`board.h` 中已配置的关键引脚：

| 功能 | GPIO |
| :--- | :---: |
| LCD SCLK / MOSI / DC / CS / 背光 | 40 / 45 / 41 / 42 / 13 |
| 触摸 SCL / SDA / INT | 3 / 1 / 4 |
| IM0 / IM1（3.5 英寸均为高电平） | 47 / 48 |
| BOOT 按键 | 0 |

## 常见问题

**底部没有 ESP-IDF 的编译/烧录按钮**  
插件未识别到工程。请关闭窗口后，重新「打开文件夹」到 `UEED_28_lvgl_port`，再运行 **Doctor Command**。

**找不到 COM 口**  
安装 CH340 驱动；换一根能传数据的 USB 线；换电脑 USB 口；关闭占用该端口的串口助手。

**烧录失败：`Failed to connect to ESP32-S3`**  
按住 BOOT 再 RESET 进入下载模式后重试。确认监视窗口已关闭，没有其它软件占用串口。

**报错分区太大 / flash size 不够**  
在 SDK Configuration 里把 Flash size 改成 **16 MB**。本示例应用分区为 12 MB。

**白屏、花屏或完全不亮**  
确认 `VIEWE_35_T` 为 `1`；排线插紧且金面朝上；3.5 英寸屏需要 IM0、IM1 拉高（`board.c` 中已处理）。

**第一次编译一直停在下载组件**  
需要能访问组件仓库。国内网络较慢时可配置 ESP 组件镜像，或改用乐鑫下载服务器安装的 IDF 环境。

**监视窗口乱码**  
波特率为 115200。重新选择正确 COM 口后再开 Monitor。

**UART 排针没有日志**  
板上 UART 与 RS485 复用。USB Type-C（CH340）仍可用于下载和打印日志。详见仓库根目录规格说明。

插件更多排错：[Troubleshooting](https://docs.espressif.com/projects/vscode-esp-idf-extension/zh_CN/latest/troubleshooting.html)。

## 命令行方式（可选）

若插件环境已经配好，也可以在 VS Code 终端或 ESP-IDF 终端里执行：

```bash
cd examples/esp_idf/UEED_28_lvgl_port
idf.py set-target esp32s3
idf.py build
idf.py -p 端口号 flash monitor
```

Windows 端口号形如 `COM3`，Linux / macOS 形如 `/dev/ttyUSB0`。

技术支持：smartrd1@viewedisplay.com
