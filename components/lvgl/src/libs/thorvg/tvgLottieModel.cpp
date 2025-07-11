/*
 * Copyright (c) 2023 - 2024 the ThorVG project. All rights reserved.

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

#include "tvgMath.h"
#include "tvgPaint.h"
#include "tvgFill.h"
#include "tvgTaskScheduler.h"
#include "tvgLottieModel.h"


/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/



/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

void LottieSlot::reset()
{
    if (!overridden) return;

    for (auto pair = pairs.begin(); pair < pairs.end(); ++pair) {
        switch (type) {
            case LottieProperty::Type::ColorStop: {
                static_cast<LottieGradient*>(pair->obj)->colorStops.release();
                static_cast<LottieGradient*>(pair->obj)->colorStops = *static_cast<LottieColorStop*>(pair->prop);
                static_cast<LottieColorStop*>(pair->prop)->frames = nullptr;
                break;
            }
            case LottieProperty::Type::Color: {
                static_cast<LottieSolid*>(pair->obj)->color.release();
                static_cast<LottieSolid*>(pair->obj)->color = *static_cast<LottieColor*>(pair->prop);
                static_cast<LottieColor*>(pair->prop)->frames = nullptr;
                break;
            }
            case LottieProperty::Type::TextDoc: {
                static_cast<LottieText*>(pair->obj)->doc.release();
                static_cast<LottieText*>(pair->obj)->doc = *static_cast<LottieTextDoc*>(pair->prop);
                static_cast<LottieTextDoc*>(pair->prop)->frames = nullptr;
                break;
            }
            default: break;
        }
        delete(pair->prop);
        pair->prop = nullptr;
    }
    overridden = false;
}


void LottieSlot::assign(LottieObject* target)
{
    //apply slot object to all targets
    for (auto pair = pairs.begin(); pair < pairs.end(); ++pair) {
        //backup the original properties before overwriting
        switch (type) {
            case LottieProperty::Type::ColorStop: {
                if (!overridden) {
                    pair->prop = new LottieColorStop;
                    *static_cast<LottieColorStop*>(pair->prop) = static_cast<LottieGradient*>(pair->obj)->colorStops;
                }

                pair->obj->override(&static_cast<LottieGradient*>(target)->colorStops);
                break;
            }
            case LottieProperty::Type::Color: {
                if (!overridden) {
                    pair->prop = new LottieColor;
                    *static_cast<LottieColor*>(pair->prop) = static_cast<LottieSolid*>(pair->obj)->color;
                }

                pair->obj->override(&static_cast<LottieSolid*>(target)->color);
                break;
            }
            case LottieProperty::Type::TextDoc: {
                if (!overridden) {
                    pair->prop = new LottieTextDoc;
                    *static_cast<LottieTextDoc*>(pair->prop) = static_cast<LottieText*>(pair->obj)->doc;
                }

                pair->obj->override(&static_cast<LottieText*>(target)->doc);
                break;
            }
            default: break;
        }
    }
    overridden = true;
}


void LottieTextRange::range(float frameNo, float totalLen, float& start, float& end)
{
    auto divisor = (rangeUnit == Unit::Percent) ? (100.0f / totalLen) : 1.0f;
    auto offset = this->offset(frameNo) / divisor;
    start = nearbyintf(this->start(frameNo) / divisor) + offset;
    end = nearbyintf(this->end(frameNo) / divisor) + offset;

    if (start > end) std::swap(start, end);

    if (random == 0) return;

    auto range = end - start;
    auto len = (rangeUnit == Unit::Percent) ? 100.0f : totalLen;
    start = static_cast<float>(random % int(len - range));
    end = start + range;
}


LottieImage::~LottieImage()
{
    lv_free(b64Data);
    lv_free(mimeType);
}


void LottieImage::prepare()
{
    LottieObject::type = LottieObject::Image;

    auto picture = Picture::gen().release();

    //force to load a picture on the same thread
    TaskScheduler::async(false);

    if (size > 0) picture->load((const char*)b64Data, size, mimeType, false);
    else picture->load(path);

    TaskScheduler::async(true);

    picture->size(width, height);
    PP(picture)->ref();

    pooler.push(picture);
}


void LottieTrimpath::segment(float frameNo, float& start, float& end, LottieExpressions* exps)
{
    start = this->start(frameNo, exps) * 0.01f;
    tvg::clamp(start, 0.0f, 1.0f);
    end = this->end(frameNo, exps) * 0.01f;
    tvg::clamp(end, 0.0f, 1.0f);

    auto o = fmodf(this->offset(frameNo, exps), 360.0f) / 360.0f;  //0 ~ 1

    auto diff = fabs(start - end);
    if (tvg::zero(diff)) {
        start = 0.0f;
        end = 0.0f;
        return;
    }
    if (tvg::equal(diff, 1.0f) || tvg::equal(diff, 2.0f)) {
        start = 0.0f;
        end = 1.0f;
        return;
    }

    if (start > end) std::swap(start, end);
    start += o;
    end += o;
}


uint32_t LottieGradient::populate(ColorStop& color, size_t count)
{
    if (!color.input) return 0;

    uint32_t alphaCnt = (color.input->count - (count * 4)) / 2;
    Array<Fill::ColorStop> output(count + alphaCnt);
    uint32_t cidx = 0;               //color count
    uint32_t clast = count * 4;
    if (clast > color.input->count) clast = color.input->count;
    uint32_t aidx = clast;           //alpha count
    Fill::ColorStop cs;

    //merge color stops.
    for (uint32_t i = 0; i < color.input->count; ++i) {
        if (cidx == clast || aidx == color.input->count) break;
        if ((*color.input)[cidx] == (*color.input)[aidx]) {
            cs.offset = (*color.input)[cidx];
            cs.r = (uint8_t)nearbyint((*color.input)[cidx + 1] * 255.0f);
            cs.g = (uint8_t)nearbyint((*color.input)[cidx + 2] * 255.0f);
            cs.b = (uint8_t)nearbyint((*color.input)[cidx + 3] * 255.0f);
            cs.a = (uint8_t)nearbyint((*color.input)[aidx + 1] * 255.0f);
            cidx += 4;
            aidx += 2;
        } else if ((*color.input)[cidx] < (*color.input)[aidx]) {
            cs.offset = (*color.input)[cidx];
            cs.r = (uint8_t)nearbyint((*color.input)[cidx + 1] * 255.0f);
            cs.g = (uint8_t)nearbyint((*color.input)[cidx + 2] * 255.0f);
            cs.b = (uint8_t)nearbyint((*color.input)[cidx + 3] * 255.0f);
            //generate alpha value
            if (output.count > 0) {
                auto p = ((*color.input)[cidx] - output.last().offset) / ((*color.input)[aidx] - output.last().offset);
                cs.a = lerp<uint8_t>(output.last().a, (uint8_t)nearbyint((*color.input)[aidx + 1] * 255.0f), p);
            } else cs.a = (uint8_t)nearbyint((*color.input)[aidx + 1] * 255.0f);
            cidx += 4;
        } else {
            cs.offset = (*color.input)[aidx];
            cs.a = (uint8_t)nearbyint((*color.input)[aidx + 1] * 255.0f);
            //generate color value
            if (output.count > 0) {
                auto p = ((*color.input)[aidx] - output.last().offset) / ((*color.input)[cidx] - output.last().offset);
                cs.r = lerp<uint8_t>(output.last().r, (uint8_t)nearbyint((*color.input)[cidx + 1] * 255.0f), p);
                cs.g = lerp<uint8_t>(output.last().g, (uint8_t)nearbyint((*color.input)[cidx + 2] * 255.0f), p);
                cs.b = lerp<uint8_t>(output.last().b, (uint8_t)nearbyint((*color.input)[cidx + 3] * 255.0f), p);
            } else {
                cs.r = (uint8_t)nearbyint((*color.input)[cidx + 1] * 255.0f);
                cs.g = (uint8_t)nearbyint((*color.input)[cidx + 2] * 255.0f);
                cs.b = (uint8_t)nearbyint((*color.input)[cidx + 3] * 255.0f);
            }
            aidx += 2;
        }
        output.push(cs);
    }

    //color remains
    while (cidx + 3 < clast) {
        cs.offset = (*color.input)[cidx];
        cs.r = (uint8_t)nearbyint((*color.input)[cidx + 1] * 255.0f);
        cs.g = (uint8_t)nearbyint((*color.input)[cidx + 2] * 255.0f);
        cs.b = (uint8_t)nearbyint((*color.input)[cidx + 3] * 255.0f);
        cs.a = (output.count > 0) ? output.last().a : 255;
        output.push(cs);
        cidx += 4;
    }

    //alpha remains
    while (aidx < color.input->count) {
        cs.offset = (*color.input)[aidx];
        cs.a = (uint8_t)nearbyint((*color.input)[aidx + 1] * 255.0f);
        if (output.count > 0) {
            cs.r = output.last().r;
            cs.g = output.last().g;
            cs.b = output.last().b;
        } else cs.r = cs.g = cs.b = 255;
        output.push(cs);
        aidx += 2;
    }

    color.data = output.data;
    output.data = nullptr;

    color.input->reset();
    delete(color.input);

    return output.count;
}


Fill* LottieGradient::fill(float frameNo, LottieExpressions* exps)
{
    auto opacity = this->opacity(frameNo);
    if (opacity == 0) return nullptr;

    Fill* fill = nullptr;
    auto s = start(frameNo, exps);
    auto e = end(frameNo, exps);

    //Linear Graident
    if (id == 1) {
        fill = LinearGradient::gen().release();
        static_cast<LinearGradient*>(fill)->linear(s.x, s.y, e.x, e.y);
    }
    //Radial Gradient
    if (id == 2) {
        fill = RadialGradient::gen().release();

        auto w = fabsf(e.x - s.x);
        auto h = fabsf(e.y - s.y);
        auto r = (w > h) ? (w + 0.375f * h) : (h + 0.375f * w);
        auto progress = this->height(frameNo, exps) * 0.01f;

        if (tvg::zero(progress)) {
            P(static_cast<RadialGradient*>(fill))->radial(s.x, s.y, r, s.x, s.y, 0.0f);
        } else {
            if (tvg::equal(progress, 1.0f)) progress = 0.99f;
            auto startAngle = rad2deg(tvg::atan2(e.y - s.y, e.x - s.x));
            auto angle = deg2rad((startAngle + this->angle(frameNo, exps)));
            auto fx = s.x + cos(angle) * progress * r;
            auto fy = s.y + sin(angle) * progress * r;
            // Lottie doesn't have any focal radius concept
            P(static_cast<RadialGradient*>(fill))->radial(s.x, s.y, r, fx, fy, 0.0f);
        }
    }

    if (!fill) return nullptr;

    colorStops(frameNo, fill, exps);

    //multiply the current opacity with the fill
    if (opacity < 255) {
        const Fill::ColorStop* colorStops;
        auto cnt = fill->colorStops(&colorStops);
        for (uint32_t i = 0; i < cnt; ++i) {
            const_cast<Fill::ColorStop*>(&colorStops[i])->a = MULTIPLY(colorStops[i].a, opacity);
        }
    }

    return fill;
}


LottieGroup::LottieGroup()
{
    reqFragment = false;
    buildDone = false;
    trimpath = false;
    visible = false;
    allowMerge = true;
}


void LottieGroup::prepare(LottieObject::Type type)
{
    LottieObject::type = type;

    if (children.count == 0) return;

    size_t strokeCnt = 0;
    size_t fillCnt = 0;

    for (auto c = children.end() - 1; c >= children.begin(); --c) {
        auto child = static_cast<LottieObject*>(*c);

        if (child->type == LottieObject::Type::Trimpath) trimpath = true;

        /* Figure out if this group is a simple path drawing.
           In that case, the rendering context can be sharable with the parent's. */
        if (allowMerge && (child->type == LottieObject::Group || !child->mergeable())) allowMerge = false;

        //Figure out this group has visible contents
        switch (child->type) {
            case LottieObject::Group: {
                visible |= static_cast<LottieGroup*>(child)->visible;
                break;
            }
            case LottieObject::Rect:
            case LottieObject::Ellipse:
            case LottieObject::Path:
            case LottieObject::Polystar:
            case LottieObject::Image:
            case LottieObject::Text: {
                visible = true;
                break;
            }
            default: break;
        }

        if (reqFragment) continue;

        /* Figure out if the rendering context should be fragmented.
           Multiple stroking or grouping with a stroking would occur this.
           This fragment resolves the overlapped stroke outlines. */
        if (child->type == LottieObject::Group && !child->mergeable()) {
            if (strokeCnt > 0 || fillCnt > 0) reqFragment = true;
        } else if (child->type == LottieObject::SolidStroke || child->type == LottieObject::GradientStroke) {
            if (strokeCnt > 0) reqFragment = true;
            else ++strokeCnt;
        } else if (child->type == LottieObject::SolidFill || child->type == LottieObject::GradientFill) {
            if (fillCnt > 0) reqFragment = true;
            else ++fillCnt;
        }
    }

    //Reverse the drawing order if this group has a trimpath.
    if (!trimpath) return;

    for (uint32_t i = 0; i < children.count - 1; ) {
        auto child2 = children[i + 1];
        if (!child2->mergeable() || child2->type == LottieObject::Transform) {
            i += 2;
            continue;
        }
        auto child = children[i];
        if (!child->mergeable() || child->type == LottieObject::Transform) {
            i++;
            continue;
        }
        children[i] = child2;
        children[i + 1] = child;
        i++;
    }
}


LottieLayer::~LottieLayer()
{
    //No need to free assets children because the Composition owns them.
    if (rid) children.clear();

    for (auto m = masks.begin(); m < masks.end(); ++m) {
        delete(*m);
    }

    for (auto e = effects.begin(); e < effects.end(); ++e) {
        delete(*e);
    }

    delete(transform);
    lv_free(name);
}


void LottieLayer::prepare(RGB24* color)
{
    /* if layer is hidden, only useful data is its transform matrix.
       so force it to be a Null Layer and release all resource. */
    if (hidden) {
        type = LottieLayer::Null;
        for (auto p = children.begin(); p < children.end(); ++p) delete(*p);
        children.reset();
        return;
    }

    //prepare the viewport clipper
    if (type == LottieLayer::Precomp) {
        auto clipper = Shape::gen().release();
        clipper->appendRect(0.0f, 0.0f, w, h);
        PP(clipper)->ref();
        statical.pooler.push(clipper);
    //prepare solid fill in advance if it is a layer type.
    } else if (color && type == LottieLayer::Solid) {
        auto solidFill = Shape::gen().release();
        solidFill->appendRect(0, 0, static_cast<float>(w), static_cast<float>(h));
        solidFill->fill(color->rgb[0], color->rgb[1], color->rgb[2]);
        PP(solidFill)->ref();
        statical.pooler.push(solidFill);
    }

    LottieGroup::prepare(LottieObject::Layer);
}


float LottieLayer::remap(LottieComposition* comp, float frameNo, LottieExpressions* exp)
{
    if (timeRemap.frames || timeRemap.value) {
        frameNo = comp->frameAtTime(timeRemap(frameNo, exp));
    } else {
        frameNo -= startFrame;
    }
    return (frameNo / timeStretch);
}


LottieComposition::~LottieComposition()
{
    if (!initiated && root) delete(root->scene);

    delete(root);
    lv_free(version);
    lv_free(name);

    //delete interpolators
    for (auto i = interpolators.begin(); i < interpolators.end(); ++i) {
    	lv_free((*i)->key);
    	lv_free(*i);
    }

    //delete assets
    for (auto a = assets.begin(); a < assets.end(); ++a) {
        delete(*a);
    }

    //delete fonts
    for (auto f = fonts.begin(); f < fonts.end(); ++f) {
        delete(*f);
    }

    //delete slots
    for (auto s = slots.begin(); s < slots.end(); ++s) {
        delete(*s);
    }

    for (auto m = markers.begin(); m < markers.end(); ++m) {
        delete(*m);
    }
}

#endif /* LV_USE_THORVG_INTERNAL */

