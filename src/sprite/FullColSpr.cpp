// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "FullColSpr.hpp"
#include "ImageObject.hpp"
#include "tween/Tweenable.hpp"
#include "bt/NTreeState.h"
#include "bt/NTree.h"
#include "RectT.hpp"

#include <32blit.hpp>

#include <cassert>

namespace dang
{
    FullColSpr::FullColSpr() : ColSpr(), ImageObject(), MotionObject(), TweenObject()
    {
    }

    FullColSpr::~FullColSpr()
    {
        removeAnimation(true);
        removeTweens(true);
    }

    FullColSpr::FullColSpr(const FullColSpr &s) : ColSpr(s), ImageObject(s), MotionObject(s), TweenObject(s)
    {
        // init animation
        FullColSpr::visitInit(*getAnimation().get());

        // init tweens
        for (const auto& t : getTweenList())
        {
            FullColSpr::visitInit(*t.get());
        }

        // clone the BT as well, but with a new state object
        if(s._nTreeState != nullptr && s._nTreeState->_tree != nullptr)
        {
            setNTreeState(std::make_shared<dang::NTreeState>(s._nTreeState->_tree));
        }

    }

    FullColSpr::FullColSpr(const tmx_spriteobject* so, const spImagesheet& is) : ColSpr(so), ImageObject(so, is), MotionObject()
    {
    }

    void FullColSpr::coreUpdate(uint32_t dt)
    {
        updateTweens(dt);
        SpriteObject::setPos(getPos() + updateMotion(dt));
        updateAnimation(dt);

    }

    void FullColSpr::update(uint32_t dt)
    {
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


    void FullColSpr::collide(const manifold &mf)
    {
        switch (_cr)
        {
            case CR_BOUNCE:
                if (mf.me.get() == this)
                {
                    if (mf.normalMe.x * getVel().x > 0)
                    {
                        setVelX(-getVel().x);
                    }
                    else if (mf.normalMe.y * getVel().y > 0)
                    {
                        setVelY(-getVel().y);
                    }

                }
                else
                {
                    if (mf.normalOther.x * getVel().x > 0)
                    {
                        setVelX(-getVel().x);
                    }
                    else if (mf.normalOther.y * getVel().y > 0)
                    {
                        setVelY(-getVel().y);
                    }

                }
                break;
            case CR_SLIDE:
                if (mf.normalMe.x != 0)
                {
                    setVelY(0);
                }
                else
                {
                    setVelX(0);
                }
                break;
            case CR_TOUCH:
                setVel(0,0);
                break;
        }
    }

    void FullColSpr::render(int32_t vpx, int32_t vpy)
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





    void FullColSpr::setNTreeState(spNTreeState ts)
    {
        _nTreeState = move(ts);
    }

    spNTreeState& FullColSpr::getNTreeState()
    {
        return _nTreeState;
    }

    void FullColSpr::visitInit(Tweenable &tw)
    {
        tw.init(*this);
    }

    void FullColSpr::visitUpdate(Tweenable &tw, uint32_t dt)
    {
        tw.update(*this, dt);
    }


}