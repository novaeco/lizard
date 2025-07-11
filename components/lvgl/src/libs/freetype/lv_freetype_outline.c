/**
 * @file lv_freetype_outline.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "../../misc/lv_event_private.h"
#include "../../lvgl.h"
#include "lv_freetype_private.h"

#if LV_USE_FREETYPE

/*********************
 *      DEFINES
 *********************/

#define CACHE_NAME "FREETYPE_OUTLINE"

/**********************
 *      TYPEDEFS
 **********************/

typedef struct _lv_freetype_outline_node_t {
    FT_UInt glyph_index;
    lv_freetype_outline_t outline;
} lv_freetype_outline_node_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

static lv_freetype_outline_t outline_create(lv_freetype_context_t * ctx, FT_Face face, FT_UInt glyph_index,
                                            uint32_t size, uint32_t strength, uint32_t border_width);
static lv_result_t outline_delete(lv_freetype_context_t * ctx, lv_freetype_outline_t outline);
static const void * freetype_get_glyph_bitmap_cb(lv_font_glyph_dsc_t * g_dsc, lv_draw_buf_t * draw_buf);
static void freetype_release_glyph_cb(const lv_font_t * font, lv_font_glyph_dsc_t * g_dsc);

static lv_cache_entry_t * lv_freetype_outline_lookup(lv_freetype_font_dsc_t * dsc, FT_UInt glyph_index);

/*glyph dsc cache lru callbacks*/
static bool freetype_glyph_outline_create_cb(lv_freetype_outline_node_t * node, lv_freetype_font_dsc_t * dsc);
static void freetype_glyph_outline_free_cb(lv_freetype_outline_node_t * node, lv_freetype_font_dsc_t * dsc);
static lv_cache_compare_res_t freetype_glyph_outline_cmp_cb(const lv_freetype_outline_node_t * node_a,
                                                            const lv_freetype_outline_node_t * node_b);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_cache_t * lv_freetype_create_draw_data_outline(uint32_t cache_size)
{
    lv_cache_ops_t glyph_outline_cache_ops = {
        .create_cb = (lv_cache_create_cb_t)freetype_glyph_outline_create_cb,
        .free_cb = (lv_cache_free_cb_t)freetype_glyph_outline_free_cb,
        .compare_cb = (lv_cache_compare_cb_t)freetype_glyph_outline_cmp_cb,
    };

    lv_cache_t * draw_data_cache = lv_cache_create(&lv_cache_class_lru_rb_count, sizeof(lv_freetype_outline_node_t),
                                                   cache_size,
                                                   glyph_outline_cache_ops);
    lv_cache_set_name(draw_data_cache, CACHE_NAME);

    return draw_data_cache;
}

void lv_freetype_set_cbs_outline_font(lv_freetype_font_dsc_t * dsc)
{
    LV_ASSERT_FREETYPE_FONT_DSC(dsc);
    dsc->font.get_glyph_bitmap = freetype_get_glyph_bitmap_cb;
    dsc->font.release_glyph = freetype_release_glyph_cb;
}

void lv_freetype_outline_add_event(lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data)
{
    LV_UNUSED(user_data);
    lv_freetype_context_t * ctx = lv_freetype_get_context();

    LV_UNUSED(filter);
    ctx->event_cb = event_cb;
}

uint32_t lv_freetype_outline_get_scale(const lv_font_t * font)
{
    LV_ASSERT_NULL(font);
    const lv_freetype_font_dsc_t * dsc = font->dsc;
    LV_ASSERT_FREETYPE_FONT_DSC(dsc);

    return FT_INT_TO_F26DOT6(dsc->size) / dsc->cache_node->ref_size;
}

bool lv_freetype_is_outline_font(const lv_font_t * font)
{
    LV_ASSERT_NULL(font);
    const lv_freetype_font_dsc_t * dsc = font->dsc;
    if(!LV_FREETYPE_FONT_DSC_HAS_MAGIC_NUM(dsc)) {
        return false;
    }

    return dsc->render_mode == LV_FREETYPE_FONT_RENDER_MODE_OUTLINE;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*-------------------
 *   OUTLINE CACHE
 *------------------*/

static bool freetype_glyph_outline_create_cb(lv_freetype_outline_node_t * node, lv_freetype_font_dsc_t * dsc)
{
    LV_PROFILER_FONT_BEGIN;
    lv_freetype_outline_t outline;

    lv_mutex_lock(&dsc->cache_node->face_lock);
    outline = outline_create(dsc->context,
                             dsc->cache_node->face,
                             node->glyph_index,
                             dsc->cache_node->ref_size,
                             dsc->style & LV_FREETYPE_FONT_STYLE_BOLD ? 1 : 0,
                             dsc->outline_stroke_width);
    lv_mutex_unlock(&dsc->cache_node->face_lock);

    if(!outline) {
        LV_PROFILER_FONT_END;
        return false;
    }

    LV_LOG_INFO("glyph_index = 0x%" LV_PRIx32, (uint32_t)node->glyph_index);

    node->outline = outline;
    LV_PROFILER_FONT_END;
    return true;
}

static void freetype_glyph_outline_free_cb(lv_freetype_outline_node_t * node, lv_freetype_font_dsc_t * dsc)
{
    LV_UNUSED(dsc);

    lv_freetype_outline_t outline = node->outline;
    lv_freetype_context_t * ctx = lv_freetype_get_context();
    outline_delete(ctx, outline);
}

static lv_cache_compare_res_t freetype_glyph_outline_cmp_cb(const lv_freetype_outline_node_t * node_a,
                                                            const lv_freetype_outline_node_t * node_b)
{
    if(node_a->glyph_index == node_b->glyph_index) {
        return 0;
    }
    return node_a->glyph_index > node_b->glyph_index ? 1 : -1;
}

static const void * freetype_get_glyph_bitmap_cb(lv_font_glyph_dsc_t * g_dsc, lv_draw_buf_t * draw_buf)
{
    LV_UNUSED(draw_buf);

    const lv_font_t * font = g_dsc->resolved_font;
    lv_freetype_font_dsc_t * dsc = (lv_freetype_font_dsc_t *)font->dsc;
    LV_ASSERT_FREETYPE_FONT_DSC(dsc);

    dsc->outline_stroke_width = g_dsc->outline_stroke_width;

    lv_cache_entry_t * entry = lv_freetype_outline_lookup(dsc, (FT_UInt)g_dsc->gid.index);

    if(entry == NULL) {
        return NULL;
    }
    lv_freetype_outline_node_t * node = lv_cache_entry_get_data(entry);

    g_dsc->entry = entry;

    return node ? node->outline : NULL;
}

static void freetype_release_glyph_cb(const lv_font_t * font, lv_font_glyph_dsc_t * g_dsc)
{
    LV_ASSERT_NULL(font);
    lv_freetype_font_dsc_t * dsc = (lv_freetype_font_dsc_t *)font->dsc;

    if(g_dsc->entry == NULL) {
        return;
    }
    lv_cache_release(dsc->cache_node->draw_data_cache, g_dsc->entry, NULL);
    g_dsc->entry = NULL;
}

static lv_cache_entry_t * lv_freetype_outline_lookup(lv_freetype_font_dsc_t * dsc, FT_UInt glyph_index)
{
    LV_PROFILER_FONT_BEGIN;
    lv_freetype_cache_node_t * cache_node = dsc->cache_node;

    lv_freetype_outline_node_t tmp_node;
    tmp_node.glyph_index = glyph_index;

    lv_cache_entry_t * entry = lv_cache_acquire_or_create(cache_node->draw_data_cache, &tmp_node, dsc);
    if(!entry) {
        LV_LOG_ERROR("glyph outline lookup failed for glyph_index = 0x%" LV_PRIx32, (uint32_t)glyph_index);
        LV_PROFILER_FONT_END;
        return NULL;
    }
    LV_PROFILER_FONT_END;
    return entry;
}

static void ft_vector_to_lv_vector(lv_freetype_outline_vector_t * dest, const FT_Vector * src)
{
    dest->x = src ? src->x : 0;
    dest->y = src ? src->y : 0;
}

static lv_result_t outline_send_event(lv_freetype_context_t * ctx, lv_event_code_t code,
                                      lv_freetype_outline_event_param_t * param)
{
    if(!ctx->event_cb) {
        LV_LOG_ERROR("event_cb is not set");
        return LV_RESULT_INVALID;
    }

    lv_event_t e;
    lv_memzero(&e, sizeof(e));
    e.code = code;
    e.param = param;
    e.user_data = NULL;

    ctx->event_cb(&e);

    return LV_RESULT_OK;
}

static lv_result_t outline_push_point(
    lv_freetype_outline_t outline,
    lv_freetype_outline_type_t type,
    const FT_Vector * control1,
    const FT_Vector * control2,
    const FT_Vector * to)
{
    LV_PROFILER_FONT_BEGIN;
    lv_freetype_context_t * ctx = lv_freetype_get_context();

    lv_freetype_outline_event_param_t param;
    lv_memzero(&param, sizeof(param));
    param.outline = outline;
    param.type = type;
    ft_vector_to_lv_vector(&param.control1, control1);
    ft_vector_to_lv_vector(&param.control2, control2);
    ft_vector_to_lv_vector(&param.to, to);

    LV_PROFILER_FONT_END;
    return outline_send_event(ctx, LV_EVENT_INSERT, &param);
}

static int outline_move_to_cb(
    const FT_Vector * to,
    void * user)
{
    lv_freetype_outline_t outline = user;
    outline_push_point(outline, LV_FREETYPE_OUTLINE_MOVE_TO, NULL, NULL, to);
    return FT_Err_Ok;
}

static int outline_line_to_cb(
    const FT_Vector * to,
    void * user)
{
    lv_freetype_outline_t outline = user;
    outline_push_point(outline, LV_FREETYPE_OUTLINE_LINE_TO, NULL, NULL, to);
    return FT_Err_Ok;
}

static int outline_conic_to_cb(
    const FT_Vector * control,
    const FT_Vector * to,
    void * user)
{
    lv_freetype_outline_t outline = user;
    outline_push_point(outline, LV_FREETYPE_OUTLINE_CONIC_TO, control, NULL, to);
    return FT_Err_Ok;
}

static int outline_cubic_to_cb(
    const FT_Vector * control1,
    const FT_Vector * control2,
    const FT_Vector * to,
    void * user)
{
    lv_freetype_outline_t outline = user;
    outline_push_point(outline, LV_FREETYPE_OUTLINE_CUBIC_TO, control1, control2, to);
    return FT_Err_Ok;
}

static lv_freetype_outline_t outline_create(
    lv_freetype_context_t * ctx,
    FT_Face face,
    FT_UInt glyph_index,
    uint32_t size,
    uint32_t strength,
    uint32_t border_width)
{
    LV_PROFILER_FONT_BEGIN;
    LV_ASSERT_NULL(ctx);
    FT_Error error;
    FT_Glyph glyph;
    FT_Stroker stroker;

    error = FT_Set_Pixel_Sizes(face, 0, size);
    if(error) {
        FT_ERROR_MSG("FT_Set_Char_Size", error);
        LV_PROFILER_FONT_END;
        return NULL;
    }


    /**
     * Disable AUTOHINT(https://freetype.org/autohinting/hinter.html) to avoid display clipping
     * caused by inconsistent glyph measurement and outline.
     */
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP | FT_LOAD_NO_AUTOHINT);
    if(error) {
        FT_ERROR_MSG("FT_Load_Glyph", error);
        LV_PROFILER_FONT_END;
        return NULL;
    }

    if(strength > 0) {
        error = FT_Outline_Embolden(&face->glyph->outline, FT_INT_TO_F26DOT6(strength));
        if(error) {
            FT_ERROR_MSG("FT_Outline_Embolden", error);
        }
    }


    FT_Outline_Funcs outline_funcs = {
        .move_to = outline_move_to_cb,
        .line_to = outline_line_to_cb,
        .conic_to = outline_conic_to_cb,
        .cubic_to = outline_cubic_to_cb,
        .shift = 0,
        .delta = 0
    };

    lv_result_t res;
    lv_freetype_outline_event_param_t param;
    lv_memzero(&param, sizeof(param));

    lv_freetype_outline_t outline;

    res = outline_send_event(ctx, LV_EVENT_CREATE, &param);
    outline = param.outline;

    if(res != LV_RESULT_OK || !outline) {
        LV_LOG_ERROR("Outline object create failed");
        LV_PROFILER_FONT_END;
        return NULL;
    }

    /* 1 iteration if there is no border */
    /* 2 iterations if there is a a border and the glyph itsef */
    for(int i = 0; i < (border_width > 0 ? 2 : 1); i++) {

        FT_Outline glyph_outline;

        if(i == 1) {

            /* decompose the border glyph */
            FT_Stroker_New(ctx->library, &stroker);
            FT_Stroker_Set(stroker, border_width * 64,
                           FT_STROKER_LINECAP_ROUND,
                           FT_STROKER_LINEJOIN_ROUND,
                           0);

            FT_Get_Glyph(face->glyph, &glyph);
            FT_Glyph_StrokeBorder(&glyph, stroker, 0, true);
            FT_OutlineGlyph g = (FT_OutlineGlyph) glyph;

            FT_Stroker_Done(stroker);

            glyph_outline = g->outline;

        }
        else {

            /* decompose glyph */
            glyph_outline = face->glyph->outline;
        }

        /*Calculate Total Segments Before decompose */
        int32_t tag_size = glyph_outline.n_points;
        int32_t segments = 0;
        int32_t vectors = 0;

        for(int j = 0; j < tag_size; j++) {

#if 0
            if(j == 0 && (glyph_outline.tags[j] & 0x1) == 0) {
                /* TODO handle the case where the first point is 'off curve' */
https://stackoverflow.com/questions/3465809/how-to-interpret-a-freetype-glyph-outline-when-the-first-point-on-the-contour-is
            }
#endif
            if((glyph_outline.tags[j] & 0x1) == 0x1) {
                segments++;
                vectors++;
            }
            else {
                int jj = j + 1 < tag_size ? j + 1 : 0;
                if(glyph_outline.tags[jj] & 0x1) {
                    vectors++;
                }
                else {
                    segments++;
                    vectors += 2;
                }
            }
        }

        /*Also for every contour we may have a line for close*/
        segments += glyph_outline.n_contours;
        vectors += glyph_outline.n_contours;

        param.sizes.data_size = vectors * 2;
        param.sizes.segments_size = segments;

        /* Run outline decompose again to fill outline data */
        error = FT_Outline_Decompose(&glyph_outline, &outline_funcs, outline);
        if(error) {
            FT_ERROR_MSG("FT_Outline_Decompose", error);
            outline_delete(ctx, outline);
            LV_PROFILER_FONT_END;
            return NULL;
        }

        if(i == 0 && border_width > 0) {

            /* Close the border glyph before decomposing the inside glyph */
            res = outline_push_point(outline, LV_FREETYPE_OUTLINE_BORDER_START, NULL, NULL, NULL);
            if(res != LV_RESULT_OK) {
                LV_LOG_ERROR("Outline object close failed");
                outline_delete(ctx, outline);
                LV_PROFILER_FONT_END;
                return NULL;
            }

        }
        else if(i == 0 || (i == 1 && border_width > 0)) {

            /* Close the border glyph or the regular glyph */
            res = outline_push_point(outline, LV_FREETYPE_OUTLINE_END, NULL, NULL, NULL);
            if(res != LV_RESULT_OK) {
                LV_LOG_ERROR("Outline object close failed");
                outline_delete(ctx, outline);
                LV_PROFILER_FONT_END;
                return NULL;
            }
        }
    }

    LV_PROFILER_FONT_END;
    return outline;
}

static lv_result_t outline_delete(lv_freetype_context_t * ctx, lv_freetype_outline_t outline)
{
    lv_freetype_outline_event_param_t param;
    lv_memzero(&param, sizeof(param));
    param.outline = outline;

    return outline_send_event(ctx, LV_EVENT_DELETE, &param);
}

#endif /*LV_USE_FREETYPE*/
