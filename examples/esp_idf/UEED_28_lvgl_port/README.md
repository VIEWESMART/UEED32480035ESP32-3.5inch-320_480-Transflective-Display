# UEED32480035E-WB-A LVGL Example

[中文说明](./README_CN.md)

This example drives the **UEED32480035E-WB-A** 3.5-inch 320×480 transflective touch panel with **ESP-IDF + LVGL**. After flashing, the screen shows a short usage hint. You can switch to a slideshow, change pictures, and toggle the backlight with the **BOOT** button or the touch panel.

Recommended environment: **VS Code + Espressif ESP-IDF extension**, ESP-IDF **v5.1 / v5.2 / v5.3** (this project was built with **v5.3**).

## What You Need

- UEED32480035E-WB-A development board
- USB Type-C cable (data cable, not charge-only)
- 5 V / 1 A USB power adapter or a USB port that can supply enough current
- Windows / macOS / Linux computer with internet access (first build downloads components)

## Hardware Setup

1. Insert the display FPC into the board connector. Keep the **gold contacts facing up**.
2. Connect USB Type-C between the board and the computer. The power LED should turn on.
3. For the **first flash**, or if download always fails: hold **BOOT**, press **RESET** once, then release **BOOT**. The chip stays in download mode until flashing starts.

On Windows, this board uses a **CH340C** USB-UART chip. If no new COM port appears, install the [CH340 driver](https://www.wch.cn/downloads/CH341SER_EXE.html), unplug/replug USB, then check Device Manager → Ports (COM & LPT).

## 1. Install VS Code

Download and install [Visual Studio Code](https://code.visualstudio.com/).

Also install [Git](https://git-scm.com/downloads). The ESP-IDF installer needs Git.

## 2. Install the ESP-IDF Extension

1. Open VS Code.
2. Click **Extensions** on the left (or press `Ctrl+Shift+X` / `Cmd+Shift+X`).
3. Search for **ESP-IDF**.
4. Install **ESP-IDF** published by **Espressif Systems**.

Official docs: [ESP-IDF Extension for VS Code](https://docs.espressif.com/projects/vscode-esp-idf-extension/en/latest/).

## 3. Install ESP-IDF with Espressif EIM (one-click)

The VS Code extension is only the UI. Install the SDK and toolchain with Espressif’s **ESP-IDF Installation Manager (EIM)** GUI. Do not configure IDF paths by hand.

Docs: [EIM](https://docs.espressif.com/projects/idf-im-ui/en/latest/) · [Windows setup](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/windows-setup.html)

### 3.1 Download and install EIM

Open the [EIM download page](https://dl.espressif.com/dl/eim/) and get the **GUI online installer**.

| OS | Recommended |
| :--- | :--- |
| Windows | Run the downloaded `.exe`, or `winget install Espressif.EIM` |
| macOS | `brew tap espressif/eim` then `brew install --cask eim-gui` |
| Linux | Follow the apt / Homebrew notes on the download page |

Then launch **ESP-IDF Installation Manager** from the Start menu or Applications.

### 3.2 One-click install (Easy Installation)

Stay online, then:

1. Under **New Installation**, click **Start Installation**.  
   If ESP-IDF has never been installed, only **New Installation** is shown.
2. Under **Easy Installation**, click **Start Easy Installation**.  
   This installs the latest stable ESP-IDF and all tools with default settings.
3. When prerequisites and path checks pass, click **Start Installation** on the **Ready to Install** page.
4. Wait until **Installation Complete**. This often takes 10–30 minutes.

> This example needs ESP-IDF **v5.1 / v5.2 / v5.3** (avoid v5.1.1). Easy Installation installs the latest stable release, which is usually fine.  
> To force **v5.3.x**, use **Expert / Custom Installation** and pick that version. Keep the install path free of spaces and non-ASCII characters.

If it fails, open **Logs** at the bottom, fix the issue, then click **Retry**.

### 3.3 Point VS Code at the new IDF

The ESP-IDF extension auto-detects EIM installs. Back in VS Code:

1. Press `F1` or `Ctrl+Shift+P` to open the Command Palette.
2. Run **`ESP-IDF: Select Current ESP-IDF Version`** and choose the version EIM just installed.
3. Optional: run **`ESP-IDF: Doctor Command`**. A clean report means the environment is ready.

## 4. Open This Example Folder

> Important: open **this example folder**, not the git repository root.

1. In VS Code: **File → Open Folder...**
2. Select:

```text
<repo>/examples/esp_idf/UEED_28_lvgl_port
```

The folder that contains the top-level `CMakeLists.txt` and `sdkconfig` is the ESP-IDF project. If you open the repo root instead, Build / Flash buttons will not work correctly.

Wait until the extension activates. The bottom status bar should show ESP-IDF items such as current target, COM port, build, flash, and monitor.

## 5. Confirm the Board Macro

Open `main/board.h` and make sure the **3.5-inch transflective** board is selected (this is the default in the repository):

```c
#define VIEWE_24_T      0
#define VIEWE_28_T      0
#define VIEWE_35_T      1   // 3.5-inch transflective 320x480
```

Only one of these macros may be `1`.

## 6. Set Chip Target to ESP32-S3

1. Command Palette → **`ESP-IDF: Set Espressif Device Target`**
2. Select **`esp32s3`**.
3. If asked for a board type, choose **ESP32-S3 chip (via ESP-PROG)** or the generic ESP32-S3 option. This product is not an official Espressif DevKit; the generic target is enough.

The status bar should show `esp32s3`.

## 7. Recommended sdkconfig

Command Palette → **`ESP-IDF: SDK Configuration Editor`** (same as `menuconfig`).

Check these items (many are already set in `sdkconfig`):

| Path | Recommended value | Why |
| :--- | :--- | :--- |
| Serial flasher config → Flash SPI mode | DIO | Matches this module |
| Serial flasher config → Flash size | **16 MB** | The partition table uses a 12 MB app slot |
| Component config → ESP PSRAM | Enable external SPI RAM | 8 MB Octal PSRAM on the module |
| SPI RAM → Mode | **Octal / OPI** | ESP32-S3-N16R8 |
| ESP System Settings → CPU frequency | **240 MHz** | Default performance |

This example’s `partitions.csv` sets the `factory` app partition to **12 MB**, so flash size **must be 16 MB**. An 8 MB setting will fail at build or flash time.

Click **Save**, then close the editor. First open of menuconfig can take a while.

## 8. Select Serial Port and Flash Method

1. Plug in the board.
2. Command Palette → **`ESP-IDF: Select Port to Use (COM, tty, usbserial)`**
   - Windows: `COMx` (CH340)
   - macOS: `/dev/cu.usbserial-xxxx`
   - Linux: `/dev/ttyUSB0` or similar
3. Command Palette → **`ESP-IDF: Select Flash Method`** → **UART**

The selected port appears on the status bar.

## 9. Build

Use either:

- Status bar **Build** button, or
- Command Palette → **`ESP-IDF: Build Your Project`**

The **first** build downloads LVGL and other components from the ESP Component Registry into `managed_components/`. Keep the network available. This can take several minutes.

Success looks like:

```text
Project build complete. To flash, run:
idf.py flash
```

If the build fails:

1. Confirm you opened `UEED_28_lvgl_port`, not the repo root.
2. Confirm target is `esp32s3`.
3. Command Palette → **`ESP-IDF: Full Clean Project`**, then build again.
4. If components failed to download, check the network or switch the component registry mirror.

## 10. Flash and Monitor

Recommended one-shot command:

1. Hold **BOOT**, press **RESET**, then release **BOOT** (first time, or whenever flash fails).
2. Command Palette → **`ESP-IDF: Build, Flash and Start a Monitor on Your Device`**
3. If asked, choose **UART**.

Or do it in three steps: **Build** → **Flash** → **Monitor**.

When flash succeeds, the chip resets, the monitor prints logs, and the LCD shows the English hint text.

Leave the monitor with `` Ctrl+] ``.

## How to Use After Boot

The same gestures work on the **BOOT button** and the **touch panel**:

| Action | Result |
| :--- | :--- |
| Double-click / double-tap | Enter slideshow |
| Single click / tap (in slideshow) | Next picture |
| Long press | Toggle backlight on/off |

Slideshow images are `EV_35_1` / `EV_35_2` / `EV_35_3` (320×480). Do not use images larger than the panel resolution.

## Project Layout

```text
UEED_28_lvgl_port/
├── CMakeLists.txt          ESP-IDF project file
├── sdkconfig               Current board config
├── partitions.csv          Partition table (12 MB app)
├── main/
│   ├── main.c              LVGL UI and button/touch logic
│   ├── board.c / board.h   LCD / touch init and GPIO map
│   ├── EV_35_*.c           320×480 slideshow images
│   └── idf_component.yml   LVGL and LCD component versions
└── components/             Local touch driver
```

Key pins (already set in `board.h`):

| Function | GPIO |
| :--- | :---: |
| LCD SCLK / MOSI / DC / CS / BL | 40 / 45 / 41 / 42 / 13 |
| Touch SCL / SDA / INT | 3 / 1 / 4 |
| IM0 / IM1 (3.5-inch = both high) | 47 / 48 |
| BOOT button | 0 |

## Troubleshooting

**VS Code does not show ESP-IDF status bar buttons**  
The ESP-IDF extension is not active. Re-open the `UEED_28_lvgl_port` folder, then run **Doctor Command**.

**No COM port**  
Install the CH340 driver, try another USB cable/port, and avoid charge-only cables.

**Flash fails: `Failed to connect to ESP32-S3`**  
Hold BOOT, press RESET, release BOOT, then flash again. Close any other serial tool that is using the same COM port (including an old Monitor session).

**Build error about flash size / partition too large**  
Set Flash size to **16 MB**. The app partition is 12 MB.

**White screen / no image**  
Confirm `VIEWE_35_T` is `1`, the FPC is fully inserted with gold contacts up, and IM0/IM1 are driven high in `board.c` for the 3.5-inch panel.

**First build is stuck downloading components**  
Need internet access. In China, configure an ESP component registry mirror if GitHub is slow.

**Monitor shows garbage characters**  
Baud rate is 115200. Re-select the correct COM port and start Monitor again.

More extension help: [Troubleshooting](https://docs.espressif.com/projects/vscode-esp-idf-extension/en/latest/troubleshooting.html).

## Command-Line Alternative

If the extension environment is already configured, you can also build from a terminal:

```bash
cd examples/esp_idf/UEED_28_lvgl_port
idf.py set-target esp32s3
idf.py build
idf.py -p PORT flash monitor
```

Replace `PORT` with `COM3`, `/dev/ttyUSB0`, and so on.

Technical support: smartrd1@viewedisplay.com
