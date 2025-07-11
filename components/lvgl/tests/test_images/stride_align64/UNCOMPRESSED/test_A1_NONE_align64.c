
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#elif defined(LV_BUILD_TEST)
#include "../lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_TEST_A1_NONE_ALIGN64
#define LV_ATTRIBUTE_TEST_A1_NONE_ALIGN64
#endif

static const
LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_TEST_A1_NONE_ALIGN64
uint8_t test_A1_NONE_align64_map[] = {

    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe0,0x00,0x00,0x00,0x1f,0xff,0xf2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe0,0x1f,0x80,0x00,0x10,0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe0,0x7f,0xe0,0x18,0x17,0xff,0xd2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe0,0xff,0xf0,0x3c,0x14,0x00,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe1,0xff,0xf8,0x3c,0x15,0xff,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe1,0xff,0xf8,0x18,0x15,0x01,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe3,0xff,0xfc,0x00,0x15,0x7d,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe3,0xff,0xfc,0x00,0x15,0x45,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe3,0xff,0xfc,0x3c,0x15,0x55,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe3,0xff,0xfc,0x7e,0x15,0x45,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe3,0xff,0xfc,0xff,0x15,0x7d,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe3,0xff,0xfc,0xff,0x15,0x01,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe1,0xff,0xf8,0xff,0x15,0xff,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe1,0xff,0xf8,0xff,0x14,0x00,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe0,0xff,0xf0,0x7e,0x17,0xff,0xd2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xe0,0x7f,0xe0,0x3c,0x10,0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x00,0x00,0x1f,0x80,0x00,0x1f,0xff,0xf2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x00,0x07,0xff,0xff,0xff,0xff,0xff,0xf2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x83,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x8f,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x8f,0xff,0x80,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x8f,0x0f,0x80,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x8f,0x07,0x80,0x0f,0x80,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x8e,0x07,0xc0,0x3f,0xc0,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x07,0xc0,0x3f,0xe0,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x81,0xff,0xc0,0x30,0xf0,0x00,0x00,0x52,0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x87,0xff,0xc0,0x30,0xf0,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x8f,0xff,0xc0,0x03,0xf0,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x8f,0x07,0xc0,0x3f,0xf0,0x0f,0x80,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0x07,0xc0,0x7f,0xf0,0x08,0xc0,0x52,0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0x07,0xc0,0x78,0xf0,0x01,0xc0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0x0f,0xc0,0x70,0xf0,0x0f,0xc0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x8f,0xff,0xe0,0x79,0xf0,0x0c,0xc0,0x52,0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x8f,0xff,0xe0,0x7f,0xf8,0x0d,0xc0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x87,0xf3,0xe0,0x3e,0x78,0x0f,0xe0,0x52,0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9f,0xff,0xff,0xff,0xff,0xff,0xe0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};

const lv_image_dsc_t test_A1_NONE_align64 = {
  .header.magic = LV_IMAGE_HEADER_MAGIC,
  .header.cf = LV_COLOR_FORMAT_A1,
  .header.flags = 0,
  .header.w = 71,
  .header.h = 60,
  .header.stride = 16,
  .data_size = sizeof(test_A1_NONE_align64_map),
  .data = test_A1_NONE_align64_map,
};

