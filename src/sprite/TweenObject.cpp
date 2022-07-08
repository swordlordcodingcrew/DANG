// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TweenObject.hpp"
#include "tween/Tweenable.hpp"

#include <cassert>

namespace dang
{

    TweenObject::TweenObject(const TweenObject &s)
    {
        // clone animation
        if (s._animation != nullptr)
        {
            setAnimation(std::make_shared<Tweenable>(*(s._animation)));
        }
        else
        {
            _animation = nullptr;
        }

        // clone tweens
        for (const auto& t : s._tweens)
        {
            _tweens.push_front(std::make_shared<Tweenable>(*t));
        }
    }


    void TweenObject::addTween(spTweenable tw)
    {
        visitInit(*tw.get());
        _tweens.push_front(tw);
    }

    void TweenObject::updateTweens(uint32_t dt)
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
                visitUpdate(*tw->get(), dt);
//                (*tw)->update(this, dt);
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

    void TweenObject::removeTween(const spTweenable &tw, bool suppressCB)
    {
        if (tw != nullptr)
        {
            tw->finish(suppressCB);
            _tweens.remove(tw);
        }
    }

    void TweenObject::removeTweens(bool suppressCB)
    {
        while (!_tweens.empty())
        {
            spTweenable &tw = _tweens.back();
            removeTween(tw, suppressCB);
        }
    }

    bool TweenObject::tweenActive(const spTweenable &tw)
    {
        auto tw_it = std::find(_tweens.begin(), _tweens.end(), tw);
        return tw_it != std::end(_tweens);
    }

    void TweenObject::setAnimation(spTweenable twa)
    {
        _animation = twa;
//        _animation->init(this);
    }

    spTweenable TweenObject::removeAnimation(bool suppressCB)
    {
        if (_animation)
        {
            _animation->finish(suppressCB);
            _animation->reset();
        }
        return std::move(_animation);
    }

    spTweenable TweenObject::swapAnimation(spTweenable new_anim, bool suppressCB)
    {
        spTweenable ret = removeAnimation(suppressCB);
        _animation = new_anim;
        return ret;
    }

    void TweenObject::updateAnimation(uint32_t dt)
    {
        // special case animation. There can be only one
        if (_animation)
        {
            visitUpdate(*_animation.get(), dt);
//            _animation->update(this, dt);
            if (_animation->isFinished())
            {
                _animation.reset();
            }
        }
    }



}