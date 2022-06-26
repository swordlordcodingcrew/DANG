// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <cassert>
#include "FullSpr.hpp"
#include "tween/Tweenable.hpp"
#include "bt/NTreeState.h"
#include "bt/NTree.h"

#include <32blit.hpp>
#include <src/RectT.hpp>

namespace dang
{
    FullSpr::FullSpr() : ColSpr(), ImageObject(), MotionObject()
    {
    }

    FullSpr::~FullSpr()
    {
        removeAnimation(true);
        removeTweens(true);
    }

    FullSpr::FullSpr(const FullSpr &s) : ColSpr(s), ImageObject(s), MotionObject(s)
    {
        // clone tweens
        for (const auto& t : s._tweens)
        {
            addTween(std::make_shared<Tweenable>(*t));
        }

        // clone the BT as well, but with a new state object
        if(s._nTreeState != nullptr && s._nTreeState->_tree != nullptr)
        {
            setNTreeState(std::make_shared<dang::NTreeState>(s._nTreeState->_tree));
        }

    }

    FullSpr::FullSpr(const tmx_spriteobject* so, const spImagesheet& is) : ColSpr(so), ImageObject(so, is), MotionObject()
    {
    }

    void FullSpr::update(uint32_t dt)
    {
        // update motion
        updateMotion(dt);

        // update tweens
        updateTweens(dt);

        // update behaviour tree
        if (_nTreeState != nullptr)
        {
            assert(_nTreeState->_tree != nullptr);
            _nTreeState->_tree->process(*this, _nTreeState, dt);

        #ifdef DANG_DEBUG_PRINT
            std::cout << "tree processed with status: " << +static_cast<std::underlying_type_t<dang::NTreeState::internal_state>>(_nTreeState->_internal_state) << " and node position: " << _nTreeState->_node << std::endl;
        #endif
        }
    }

    void FullSpr::render(int32_t vpx, int32_t vpy)
    {
        if (isVisible() && isActive() && inZone() /*&& getImagesheet() != nullptr*/)
        {
            Vector2F p = local2Global(getPos());
            blit::Point dp;
            dp.x  = int32_t(std::floor(p.x) - vpx);
            dp.y  = int32_t(std::floor(p.y) - vpy);
            blit::screen.blit(getSurface(), getBlitRect(), dp, getTransform());
        }
    }




    void FullSpr::addTween(spTweenable tw)
    {
        tw->init(this);
        _tweens.push_front(tw);
    }

    void FullSpr::updateTweens(uint32_t dt)
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

    void FullSpr::removeTween(const spTweenable& tw, bool suppressCB)
    {
        if (tw != nullptr)
        {
            tw->finish(suppressCB);
            _tweens.remove(tw);
        }
    }

    void FullSpr::removeTweens(bool suppressCB)
    {
        while (!_tweens.empty())
        {
            spTweenable& tw = _tweens.back();
            removeTween(tw, suppressCB);
        }
    }

    bool FullSpr::tweenActive(const spTweenable &tw)
    {
        auto tw_it = std::find(_tweens.begin(), _tweens.end(), tw);
        return tw_it != std::end(_tweens);
    }


    void FullSpr::setNTreeState(spNTreeState ts)
    {
        _nTreeState = move(ts);
    }

    spNTreeState& FullSpr::getNTreeState()
    {
        return _nTreeState;
    }

}