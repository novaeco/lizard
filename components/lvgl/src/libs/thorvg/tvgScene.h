/*
 * Copyright (c) 2020 - 2024 the ThorVG project. All rights reserved.

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "../../lv_conf_internal.h"
#if LV_USE_THORVG_INTERNAL

#ifndef _TVG_SCENE_H_
#define _TVG_SCENE_H_

#include "tvgMath.h"
#include "tvgPaint.h"

using std::list;

struct SceneIterator : Iterator
{
    list<Paint*>* paints;
    list<Paint*>::iterator itr;

    SceneIterator(list<Paint*>* p) : paints(p)
    {
        begin();
    }

    const Paint* next() override
    {
        if (itr == paints->end()) return nullptr;
        auto paint = *itr;
        ++itr;
        return paint;
    }

    uint32_t count() override
    {
       return paints->size();
    }

    void begin() override
    {
        itr = paints->begin();
    }
};

struct Scene::Impl
{
    list<Paint*> paints;
    RenderData rd = nullptr;
    Scene* scene = nullptr;
    RenderRegion vport = {0, 0, INT32_MAX, INT32_MAX};
    Array<RenderEffect*>* effects = nullptr;
    uint8_t opacity;         //for composition
    bool needComp = false;   //composite or not

    Impl(Scene* s) : scene(s)
    {
    }

    ~Impl()
    {
        resetEffects();

        for (auto paint : paints) {
            if (P(paint)->unref() == 0) delete(paint);
        }

        if (auto renderer = PP(scene)->renderer) {
            renderer->dispose(rd);
        }
    }

    bool needComposition(uint8_t opacity)
    {
        if (opacity == 0 || paints.empty()) return false;

        //post effects requires composition
        if (effects) return true;

        //Masking may require composition (even if opacity == 255)
        auto compMethod = scene->composite(nullptr);
        if (compMethod != CompositeMethod::None && compMethod != CompositeMethod::ClipPath) return true;

        //Blending may require composition (even if opacity == 255)
        if (PP(scene)->blendMethod != BlendMethod::Normal) return true;

        //Half translucent requires intermediate composition.
        if (opacity == 255) return false;

        //If scene has several children or only scene, it may require composition.
        //OPTIMIZE: the bitmap type of the picture would not need the composition.
        //OPTIMIZE: a single paint of a scene would not need the composition.
        if (paints.size() == 1 && paints.front()->type() == Type::Shape) return false;

        return true;
    }

    RenderData update(RenderMethod* renderer, const Matrix& transform, Array<RenderData>& clips, uint8_t opacity, RenderUpdateFlag flag, TVG_UNUSED bool clipper)
    {
        this->vport = renderer->viewport();

        if ((needComp = needComposition(opacity))) {
            /* Overriding opacity value. If this scene is half-translucent,
               It must do intermediate composition with that opacity value. */
            this->opacity = opacity;
            opacity = 255;
        }
        for (auto paint : paints) {
            paint->pImpl->update(renderer, transform, clips, opacity, flag, false);
        }

        return nullptr;
    }

    bool render(RenderMethod* renderer)
    {
        RenderCompositor* cmp = nullptr;
        auto ret = true;

        renderer->blend(PP(scene)->blendMethod);

        if (needComp) {
            cmp = renderer->target(bounds(renderer), renderer->colorSpace());
            renderer->beginComposite(cmp, CompositeMethod::None, opacity);
        }

        for (auto paint : paints) {
            ret &= paint->pImpl->render(renderer);
        }

        if (cmp) {
            //Apply post effects if any.
            if (effects) {
                for (auto e = effects->begin(); e < effects->end(); ++e) {
                    renderer->effect(cmp, *e);
                }
            }
            renderer->endComposite(cmp);
        }

        return ret;
    }

    RenderRegion bounds(RenderMethod* renderer) const
    {
        if (paints.empty()) return {0, 0, 0, 0};

        int32_t x1 = INT32_MAX;
        int32_t y1 = INT32_MAX;
        int32_t x2 = 0;
        int32_t y2 = 0;

        for (auto paint : paints) {
            auto region = paint->pImpl->bounds(renderer);

            //Merge regions
            if (region.x < x1) x1 = region.x;
            if (x2 < region.x + region.w) x2 = (region.x + region.w);
            if (region.y < y1) y1 = region.y;
            if (y2 < region.y + region.h) y2 = (region.y + region.h);
        }

        //Extends the render region if post effects require
        int32_t ex = 0, ey = 0, ew = 0, eh = 0;
        if (effects) {
            for (auto e = effects->begin(); e < effects->end(); ++e) {
                auto effect = *e;
                if (effect->rd || renderer->prepare(effect)) {
                    ex = std::min(ex, effect->extend.x);
                    ey = std::min(ey, effect->extend.y);
                    ew = std::max(ew, effect->extend.w);
                    eh = std::max(eh, effect->extend.h);
                }
            }
        }

        auto ret = RenderRegion{x1 + ex, y1 + ey, (x2 - x1) + ew, (y2 - y1) + eh};
        ret.intersect(this->vport);
        return ret;
    }

    bool bounds(float* px, float* py, float* pw, float* ph, bool stroking)
    {
        if (paints.empty()) return false;

        auto x1 = FLT_MAX;
        auto y1 = FLT_MAX;
        auto x2 = -FLT_MAX;
        auto y2 = -FLT_MAX;

        for (auto paint : paints) {
            auto x = FLT_MAX;
            auto y = FLT_MAX;
            auto w = 0.0f;
            auto h = 0.0f;

            if (!P(paint)->bounds(&x, &y, &w, &h, true, stroking)) continue;

            //Merge regions
            if (x < x1) x1 = x;
            if (x2 < x + w) x2 = (x + w);
            if (y < y1) y1 = y;
            if (y2 < y + h) y2 = (y + h);
        }

        if (px) *px = x1;
        if (py) *py = y1;
        if (pw) *pw = (x2 - x1);
        if (ph) *ph = (y2 - y1);

        return true;
    }

    Paint* duplicate(Paint* ret)
    {
        if (ret) TVGERR("RENDERER", "TODO: duplicate()");

        auto scene = Scene::gen().release();
        auto dup = scene->pImpl;

        for (auto paint : paints) {
            auto cdup = paint->duplicate();
            P(cdup)->ref();
            dup->paints.push_back(cdup);
        }

        if (effects) TVGERR("RENDERER", "TODO: Duplicate Effects?");

        return scene;
    }

    void clear(bool free)
    {
        for (auto paint : paints) {
            if (P(paint)->unref() == 0 && free) delete(paint);
        }
        paints.clear();
    }

    Iterator* iterator()
    {
        return new SceneIterator(&paints);
    }

    Result resetEffects();
};

#endif //_TVG_SCENE_H_

#endif /* LV_USE_THORVG_INTERNAL */

