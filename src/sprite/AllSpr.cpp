// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "AllSpr.hpp"
#include "tween/Tweenable.hpp"

namespace dang
{
    AllSpr::AllSpr() : ColSpr(), ImageObject(), MotionObject()
    {
    }

    AllSpr::~AllSpr()
    {
        removeAnimation(true);
        removeTweens(true);
    }

    AllSpr::AllSpr(const AllSpr &s) : ColSpr(s), ImageObject(s), MotionObject(s)
    {
        for (const auto& t : s._tweens)
        {
            addTween(std::make_shared<Tweenable>(*t));
        }

    }

    AllSpr::AllSpr(const tmx_spriteobject* so, const spImagesheet& is) : ColSpr(so), ImageObject(so, is), MotionObject()
    {
    }

    void AllSpr::addTween(spTweenable tw)
    {
        tw->init(this);
        _tweens.push_front(tw);
    }

    void AllSpr::updateTweens(uint32_t dt)
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

    void AllSpr::removeTween(const spTweenable& tw, bool suppressCB)
    {
        if (tw != nullptr)
        {
            tw->finish(suppressCB);
            _tweens.remove(tw);
        }
    }

    void AllSpr::removeTweens(bool suppressCB)
    {
        while (!_tweens.empty())
        {
            spTweenable& tw = _tweens.back();
            removeTween(tw, suppressCB);
        }
    }

    bool AllSpr::tweenActive(const spTweenable &tw)
    {
        auto tw_it = std::find(_tweens.begin(), _tweens.end(), tw);
        return tw_it != std::end(_tweens);
    }

}