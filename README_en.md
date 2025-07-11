# ESP32-S3 LVGL + LovyanGFX Multi-Board Demo

This ESP-IDF C++ project controls Waveshare ESP32-S3 touchscreen displays (3.5", 5", 7", 7" type B) with **LovyanGFX** (fast graphics driver) and **LVGL** (modern GUI) across multiple boards, with **GT911** touch support via LovyanGFX.

## Prerequisites

- ESP-IDF v5.x installed: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/
- Python 3.7+
- Git
- Waveshare ESP32-S3 board

## Dependencies

LovyanGFX (display driver + GT911 touch) is bundled in
`components/LovyanGFX`, so no additional download is required.

## Build

. $IDF_PATH/export.sh
idf.py set-target esp32s3
idf.py build flash monitor

## Selecting the board

Run `idf.py menuconfig` and choose **Board Selection** to pick your board:
- Waveshare ESP32-S3 7 inch
- Waveshare ESP32-S3 7 inch Type B
- Waveshare ESP32-S3 5 inch
- Waveshare ESP32-S3 3.5 inch

All C and C++ sources include `board_config.hpp` to obtain the pin definitions for the selected board.
If no board is chosen in `idf.py menuconfig`, compilation will stop with an error. A default board setting is provided in `sdkconfig.defaults` for the 7 inch board.

## License

This project is released under the MIT License. See [LICENSE](LICENSE) for details.

## Example interface

The application displays a slider and a button. Moving the slider updates the value shown underneath, and each button press increments a counter displayed at the top of the screen.

The latest version also features a brightness slider, a switch, a drop-down menu and a checkbox.
Moving the brightness slider immediately updates the screen backlight level.
The state of each widget and the selected option are displayed in real time.

## Theme usage

The user interface initializes LVGL's default theme in `main/ui/lvgl_ui.cpp` right after
`lv_init()`. The theme uses blue as the primary color, red as the secondary color
and the Montserrat 14 font. A basic style sets the background color and font on
widgets when they are created.
