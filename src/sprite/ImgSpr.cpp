// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "ImgSpr.hpp"
#include "tween/Tweenable.hpp"

#include <32blit.hpp>
#include <src/RectT.hpp>

namespace dang
{
    ImgSpr::ImgSpr() : SpriteObject(), ImageObject()
    {
    }

    ImgSpr::~ImgSpr()
    {
        removeAnimation(true);
        removeTweens(true);
    }

    ImgSpr::ImgSpr(const ImgSpr &s) : SpriteObject(), ImageObject()
    {
        for (const auto& t : s._tweens)
        {
            addTween(std::make_shared<Tweenable>(*t));
        }

    }

    ImgSpr::ImgSpr(const tmx_spriteobject* so, const spImagesheet& is) : SpriteObject(so), ImageObject(so, is)
    {
    }

    void ImgSpr::addTween(spTweenable tw)
    {
        tw->init(this);
        _tweens.push_front(tw);
    }

    void ImgSpr::updateTweens(uint32_t dt)
    {
        auto tw = _tweens.begin();
        while (tw != _tweens.end())
        {
            if (!(*tw))
            {
                // somehow a stale tween
                tw = _tweens.erase(tw);
            }
            else
            {
                (*tw)->update(this, dt);
                if ((*tw)->isFinished())
                {
                    tw = _tweens.erase(tw);
                }
                else
                {
                    ++tw;
                }
            }
        }
    }

    void ImgSpr::removeTween(const spTweenable& tw, bool suppressCB)
    {
        if (tw != nullptr)
        {
            tw->finish(suppressCB);
            _tweens.remove(tw);
        }
    }

    void ImgSpr::removeTweens(bool suppressCB)
    {
        while (!_tweens.empty())
        {
            spTweenable& tw = _tweens.back();
            removeTween(tw, suppressCB);
        }
    }

    bool ImgSpr::tweenActive(const spTweenable &tw)
    {
        auto tw_it = std::find(_tweens.begin(), _tweens.end(), tw);
        return tw_it != std::end(_tweens);
    }

    void ImgSpr::coreUpdate(uint32_t dt)
    {
        updateTweens(dt);
    }

    void ImgSpr::render(int32_t vpx, int32_t vpy)
    {
//        (*it)->_imagesheet != nullptr && vp.intersects((*it)->getSizeRectG())
        if (isVisible() && isActive() && inZone())
        {
            Vector2F p = local2Global(getPos());
            blit::Point dp;
            dp.x  = int32_t(std::floor(p.x) - vpx);
            dp.y  = int32_t(std::floor(p.y) - vpy);
            blit::screen.blit(getSurface(), getBlitRect(), dp, getTransform());
        }
    }

    RectF ImgSpr::getSizeRect()
    {
        return RectF(getPos().x, getPos().y, getSize().x, getSize().y);
    }

    RectF ImgSpr::getSizeRectG()
    {
        Vector2F p = local2Global(getPos());
        return RectF(p.x, p.y, getSize().x, getSize().y);
    }

}