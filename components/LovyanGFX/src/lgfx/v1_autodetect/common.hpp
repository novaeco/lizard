/*----------------------------------------------------------------------------/
  Lovyan GFX - Graphics library for embedded devices.

Original Source:
 https://github.com/lovyan03/LovyanGFX/

Licence:
 [FreeBSD](https://github.com/lovyan03/LovyanGFX/blob/master/license.txt)

Author:
 [lovyan03](https://twitter.com/lovyan03)

Contributors:
 [ciniml](https://github.com/ciniml)
 [mongonta0716](https://github.com/mongonta0716)
 [tobozo](https://github.com/tobozo)
/----------------------------------------------------------------------------*/
#pragma once

#ifndef LGFX_USE_V1
#define LGFX_USE_V1
#endif

#include "LovyanGFX.hpp"

#if defined (ESP_PLATFORM)

 #include <sdkconfig.h>

 #include "LGFX_AutoDetect_ESP32_all.hpp"

#elif defined (ESP8266)

 #include "LGFX_AutoDetect_ESP8266.hpp"

#elif defined (__SAMD21__) || defined(__SAMD21G18A__) || defined(__SAMD21J18A__) || defined(__SAMD21E17A__) || defined(__SAMD21E18A__)

 #include "LGFX_AutoDetect_SAMD21.hpp"

#elif defined (__SAMD51__)

 #include "LGFX_AutoDetect_SAMD51.hpp"

#elif defined (STM32F2xx) || defined (STM32F4xx) || defined (STM32F7xx)

  #include "LGFX_AutoDetect_STM32.hpp"

#elif __has_include(<SDL2/SDL.h>) || __has_include(<SDL.h>)

  #include "LGFX_AutoDetect_sdl.hpp"

#elif __has_include(<opencv2/opencv.hpp>)

  #include "LGFX_AutoDetect_OpenCV.hpp"

#elif defined (LGFX_LINUX_FB)

  #include "LGFX_AutoDetect_FrameBuffer.hpp"

#endif
