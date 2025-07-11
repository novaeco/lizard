#if LV_BUILD_TEST
#include "../lvgl.h"
#include "../../lvgl_private.h"

#include "unity/unity.h"

void setUp(void)
{
    /* Function run before every test */
}

void tearDown(void)
{
    /* Function run after every test */
}

void test_rotate90_RGB565(void)
{
    uint16_t srcArray[3 * 2] = {
        0x1110, 0x2220, 0x3330,
        0x4440, 0x5550, 0x6660
    };
    uint16_t dstArray[2 * 3] = {0};

    uint16_t expectedArray[2 * 3] = {
        0x3330, 0x6660,
        0x2220, 0x5550,
        0x1110, 0x4440,
    };

    lv_draw_sw_rotate(srcArray, dstArray,
                      3, 2,
                      3 * sizeof(uint16_t),
                      2 * sizeof(uint16_t),
                      LV_DISPLAY_ROTATION_90,
                      LV_COLOR_FORMAT_RGB565);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedArray, dstArray, sizeof(dstArray));
}

void test_rotate180_RGB565(void)
{
    uint16_t srcArray[3 * 2] = {
        0x1110, 0x2220, 0x3330,
        0x4440, 0x5550, 0x6660
    };
    uint16_t dstArray[3 * 2] = {0};
    uint16_t expectedArray[3 * 2] = {
        0x6660, 0x5550, 0x4440,
        0x3330, 0x2220, 0x1110,
    };
    lv_draw_sw_rotate(srcArray, dstArray,
                      3, 2,
                      3 * sizeof(uint16_t),
                      3 * sizeof(uint16_t),
                      LV_DISPLAY_ROTATION_180,
                      LV_COLOR_FORMAT_RGB565);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedArray, dstArray, sizeof(dstArray));
}

void test_rotate270_RGB565(void)
{
    uint16_t srcArray[3 * 2] = {
        0x1110, 0x2220, 0x3330,
        0x4440, 0x5550, 0x6660
    };

    uint16_t dstArray[2 * 3] = {0};

    uint16_t expectedArray[2 * 3] = {
        0x4440, 0x1110,
        0x5550, 0x2220,
        0x6660, 0x3330
    };
    lv_draw_sw_rotate(srcArray, dstArray,
                      3, 2,
                      3 * sizeof(uint16_t),
                      2 * sizeof(uint16_t),
                      LV_DISPLAY_ROTATION_270,
                      LV_COLOR_FORMAT_RGB565);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedArray, dstArray, sizeof(dstArray));
}

void test_rotate90_RGB888(void)
{
    uint8_t srcArray[3 * 2 * 3] = {
        0x11, 0x1A, 0x1B,   0x22, 0x2A, 0x2B,   0x33, 0x3A, 0x3B,
        0x44, 0x4A, 0x4B,   0x55, 0x5A, 0x5B,   0x66, 0x6A, 0x6B
    };
    uint8_t dstArray[2 * 3 * 3] = {0};

    uint8_t expectedArray[2 * 3 * 3] = {
        0x33, 0x3A, 0x3B,   0x66, 0x6A, 0x6B,
        0x22, 0x2A, 0x2B,   0x55, 0x5A, 0x5B,
        0x11, 0x1A, 0x1B,   0x44, 0x4A, 0x4B,
    };

    lv_draw_sw_rotate(srcArray, dstArray,
                      3, 2,
                      3 * 3,
                      2 * 3,
                      LV_DISPLAY_ROTATION_90,
                      LV_COLOR_FORMAT_RGB888);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedArray, dstArray, sizeof(dstArray));
}

void test_rotate180_RGB888(void)
{
    uint8_t srcArray[3 * 2 * 3] = {
        0x11, 0x1A, 0x1B,    0x22, 0x2A, 0x2B,    0x33, 0x3A, 0x3B,
        0x44, 0x4A, 0x4B,    0x55, 0x5A, 0x5B,    0x66, 0x6A, 0x6B
    };
    uint8_t dstArray[3 * 2 * 3] = {0};

    uint8_t expectedArray[3 * 2 * 3] = {
        0x66, 0x6A, 0x6B,    0x55, 0x5A, 0x5B,    0x44, 0x4A, 0x4B,
        0x33, 0x3A, 0x3B,    0x22, 0x2A, 0x2B,    0x11, 0x1A, 0x1B
    };

    lv_draw_sw_rotate(srcArray, dstArray,
                      2, 3,
                      2 * 3,
                      2 * 3,
                      LV_DISPLAY_ROTATION_180,
                      LV_COLOR_FORMAT_RGB888);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedArray, dstArray, sizeof(dstArray));
}

void test_rotate270_RGB888(void)
{
    uint8_t srcArray[3 * 2 * 3] = {
        0x11, 0x1A, 0x1B,    0x22, 0x2A, 0x2B,    0x33, 0x3A, 0x3B,
        0x44, 0x4A, 0x4B,    0x55, 0x5A, 0x5B,    0x66, 0x6A, 0x6B
    };
    uint8_t dstArray[2 * 3 * 3] = {0};

    uint8_t expectedArray[2 * 3 * 3] = {
        0x44, 0x4A, 0x4B,     0x11, 0x1A, 0x1B,
        0x55, 0x5A, 0x5B,     0x22, 0x2A, 0x2B,
        0x66, 0x6A, 0x6B,     0x33, 0x3A, 0x3B,
    };

    lv_draw_sw_rotate(srcArray, dstArray,
                      3, 2,
                      3 * 3,
                      2 * 3,
                      LV_DISPLAY_ROTATION_270,
                      LV_COLOR_FORMAT_RGB888);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedArray, dstArray, sizeof(dstArray));
}

void test_rotate90_ARGB8888(void)
{
    uint32_t srcArray[3 * 2] = {
        0x111A1B1C, 0x222A2B2C, 0x333A3B3C,
        0x444A4B4C, 0x555A5B5C, 0x666A6B6C

    };
    uint32_t dstArray[2 * 3] = {0};

    uint32_t expectedArray[2 * 3] = {
        0x333A3B3C, 0x666A6B6C,
        0x222A2B2C, 0x555A5B5C,
        0x111A1B1C, 0x444A4B4C,
    };

    lv_draw_sw_rotate(srcArray, dstArray,
                      3, 2,
                      3 * sizeof(uint32_t),
                      2 * sizeof(uint32_t),
                      LV_DISPLAY_ROTATION_90,
                      LV_COLOR_FORMAT_ARGB8888);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedArray, dstArray, sizeof(dstArray));
}

void test_rotate180_ARGB8888(void)
{
    uint32_t srcArray[3 * 2] = {
        0xFF0000FF, 0xFF00FF00, // Row 1: Red, Green
        0xFFFF0000, 0xFFFFFFFF, // Row 2: Blue, White
        0xFF00FFFF, 0xFFFFFF00  // Row 3: Cyan, Yellow
    };
    uint32_t dstArray[3 * 2] = {0};
    uint32_t expectedArray[3 * 2] = {
        0xFFFFFF00, 0xFF00FFFF, // Rotated Row 1
        0xFFFFFFFF, 0xFFFF0000, // Rotated Row 2
        0xFF00FF00, 0xFF0000FF  // Rotated Row 3
    };

    lv_draw_sw_rotate(srcArray, dstArray,
                      2, 3,
                      2 * sizeof(uint32_t),
                      2 * sizeof(uint32_t),
                      LV_DISPLAY_ROTATION_180,
                      LV_COLOR_FORMAT_ARGB8888);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedArray, dstArray, sizeof(dstArray));
}

void test_rotate270_ARGB8888(void)
{
    uint32_t srcArray[3 * 2] = {
        0x111A1B1C, 0x222A2B2C, 0x333A3B3C,
        0x444A4B4C, 0x555A5B5C, 0x666A6B6C
    };
    uint32_t dstArray[2 * 3] = {0};
    uint32_t expectedArray[2 * 3] = {
        0x444A4B4C, 0x111A1B1C,
        0x555A5B5C, 0x222A2B2C,
        0x666A6B6C, 0x333A3B3C
    };
    lv_draw_sw_rotate(srcArray, dstArray,
                      3, 2,
                      3 * sizeof(uint32_t),
                      2 * sizeof(uint32_t),
                      LV_DISPLAY_ROTATION_270,
                      LV_COLOR_FORMAT_ARGB8888);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedArray, dstArray, sizeof(dstArray));
}

void test_rotate90_L8(void)
{
    uint8_t srcArray[3 * 2] = {
        0x11, 0x22, 0x33,
        0x44, 0x55, 0x66
    };
    uint8_t dstArray[2 * 3] = {0};

    uint8_t expectedArray[2 * 3] = {
        0x33, 0x66,
        0x22, 0x55,
        0x11, 0x44,
    };

    lv_draw_sw_rotate(srcArray, dstArray,
                      3, 2,
                      3 * sizeof(uint8_t),
                      2 * sizeof(uint8_t),
                      LV_DISPLAY_ROTATION_90,
                      LV_COLOR_FORMAT_L8);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedArray, dstArray, sizeof(dstArray));
}


void test_rotate180_L8(void)
{
    uint8_t srcArray[3 * 2] = {
        0x11, 0x22, 0x33,
        0x44, 0x55, 0x66
    };
    uint8_t dstArray[3 * 2] = {0};
    uint8_t expectedArray[3 * 2] = {
        0x66, 0x55, 0x44,
        0x33, 0x22, 0x11,
    };
    lv_draw_sw_rotate(srcArray, dstArray,
                      3, 2,
                      3 * sizeof(uint8_t),
                      3 * sizeof(uint8_t),
                      LV_DISPLAY_ROTATION_180,
                      LV_COLOR_FORMAT_L8);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedArray, dstArray, sizeof(dstArray));
}

void test_rotate270_L8(void)
{
    uint8_t srcArray[3 * 2] = {
        0x11, 0x22, 0x33,
        0x44, 0x55, 0x66
    };

    uint8_t dstArray[2 * 3] = {0};

    uint8_t expectedArray[2 * 3] = {
        0x44, 0x11,
        0x55, 0x22,
        0x66, 0x33
    };
    lv_draw_sw_rotate(srcArray, dstArray,
                      3, 2,
                      3 * sizeof(uint8_t),
                      2 * sizeof(uint8_t),
                      LV_DISPLAY_ROTATION_270,
                      LV_COLOR_FORMAT_L8);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedArray, dstArray, sizeof(dstArray));
}

void test_invert(void)
{
    uint8_t expected_buf[10] = {0xff, 0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8, 0xf7, 0xf6};

    /*Aligned start and aligned end*/
    uint8_t buf1[10] =         {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    lv_draw_sw_i1_invert(buf1, 8);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buf, buf1, 8);

    /*Unaligned start and unaligned end*/
    uint8_t buf2[10] =         {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    lv_draw_sw_i1_invert(&buf2[3], 6);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&expected_buf[3], &buf2[3], 6);

    /*Small buffer*/
    uint8_t buf3[10] =         {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    lv_draw_sw_i1_invert(&buf3[3], 2);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&expected_buf[3], &buf3[3], 2);
}

void test_vtile_small(void)
{
    uint8_t src_buf[8] = {0x3C, 0x42, 0x81, 0xA5, 0x81, 0x81, 0x42, 0x3C};
    uint8_t dst_buf[8] = {0};

    uint8_t expected_buf_msb[8] = {0x3C, 0x42, 0x91, 0x81, 0x81, 0x91, 0x42, 0x3C};
    uint8_t expected_buf_lsb[8] = {0x3C, 0x42, 0x89, 0x81, 0x81, 0x89, 0x42, 0x3C};

    lv_draw_sw_i1_convert_to_vtiled(src_buf, 8, 8, 8, dst_buf, 8, false);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buf_msb, dst_buf, 8);

    lv_draw_sw_i1_convert_to_vtiled(src_buf, 8, 8, 8, dst_buf, 8, true);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buf_lsb, dst_buf, 8);
}

void test_vtile_rectangular(void)
{
    uint8_t src_buf[80] = {
        0x00, 0x00, 0x00, 0x00, 0x00,
        0x40, 0x80, 0x83, 0xE1, 0x00,
        0x40, 0x80, 0x84, 0x01, 0x00,
        0x40, 0x41, 0x08, 0x01, 0x00,
        0x40, 0x41, 0x10, 0x01, 0x00,
        0x40, 0x41, 0x20, 0x01, 0x00,
        0x40, 0x41, 0x20, 0x01, 0x00,
        0x40, 0x41, 0x20, 0x01, 0x00,
        0x40, 0x41, 0x20, 0x01, 0x00,
        0x40, 0x23, 0x20, 0x01, 0x00,
        0x40, 0x22, 0x20, 0xF1, 0x00,
        0x40, 0x14, 0x20, 0x11, 0x00,
        0x40, 0x14, 0x10, 0x11, 0x00,
        0x40, 0x08, 0x08, 0x11, 0x00,
        0x7E, 0x08, 0x07, 0xF1, 0xF8,
        0x00, 0x00, 0x00, 0x00, 0x00,
    };
    uint8_t dst_buf[80] = {0};

    uint8_t expected_buf_msb[80] = {
        0x00, 0x00,
        0x7F, 0xFE,
        0x00, 0x02,
        0x00, 0x02,
        0x00, 0x02,
        0x00, 0x02,
        0x00, 0x02,
        0x00, 0x00,
        0x60, 0x00,
        0x1F, 0x80,
        0x00, 0x60,
        0x00, 0x18,
        0x00, 0x06,
        0x00, 0x18,
        0x00, 0x60,
        0x3F, 0x80,
        0xC0, 0x00,
        0x00, 0x00,
        0x07, 0xF0,
        0x08, 0x08,
        0x10, 0x04,
        0x20, 0x02,
        0x80, 0x02,
        0x80, 0x02,
        0x80, 0x22,
        0x80, 0x22,
        0x80, 0x22,
        0x00, 0x3E,
        0x00, 0x00,
        0x00, 0x00,
        0x00, 0x00,
        0xFF, 0xFE,
        0x00, 0x02,
        0x00, 0x02,
        0x00, 0x02,
        0x00, 0x02,
        0x00, 0x02,
        0x00, 0x00,
        0x00, 0x00,
        0x00, 0x00,
    };
    uint8_t expected_buf_lsb[80] = {
        0x00, 0x00,
        0xFE, 0x7F,
        0x00, 0x40,
        0x00, 0x40,
        0x00, 0x40,
        0x00, 0x40,
        0x00, 0x40,
        0x00, 0x00,
        0x06, 0x00,
        0xF8, 0x01,
        0x00, 0x06,
        0x00, 0x18,
        0x00, 0x60,
        0x00, 0x18,
        0x00, 0x06,
        0xF8, 0x03,
        0x06, 0x00,
        0x00, 0x00,
        0xE0, 0x0F,
        0x10, 0x10,
        0x08, 0x20,
        0x04, 0x40,
        0x02, 0x40,
        0x02, 0x40,
        0x02, 0x44,
        0x02, 0x44,
        0x02, 0x44,
        0x00, 0x7C,
        0x00, 0x00,
        0x00, 0x00,
        0x00, 0x00,
        0xFE, 0x7F,
        0x00, 0x40,
        0x00, 0x40,
        0x00, 0x40,
        0x00, 0x40,
        0x00, 0x40,
        0x00, 0x00,
        0x00, 0x00,
        0x00, 0x00,
    };

    lv_draw_sw_i1_convert_to_vtiled(src_buf, 80, 40, 16,  dst_buf, 80, false);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buf_msb, dst_buf, 8);

    lv_draw_sw_i1_convert_to_vtiled(src_buf, 80, 40, 16, dst_buf, 80, true);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buf_lsb, dst_buf, 8);
}

#endif
