// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "Wavepoint.hpp"
#include "../tween/TwPos.hpp"
#include "../tween/TwPosSpline.h"


namespace dang
{
    Wavepoint::Wavepoint(uint32_t id, const std::string& name, const std::string& type, dang::Vector2F pos, uint32_t next_id, uint32_t duration, uint8_t orientation, uint32_t delay)
    : _name(name), _type(type), _next_id(next_id), _id(id), _duration(duration), _orientation(orientation), _delay(delay), _pos(pos)
    {
        _next = nullptr;
    }

    spTwPos Wavepoint::getTwPos() const
    {
        if (_next == nullptr)
        {
            return nullptr;
        }

        dang::spTwPos ret = std::make_shared<TwPos>(_next->_pos, _duration, Ease::Linear);

        return ret;
    }

    spTwPosSpline Wavepoint::getTwPosSpline() const
    {
        if (_next == nullptr)
        {
            return nullptr;
        }

        spTwPosSpline ret = std::make_shared<TwPosSpline>(this, _duration, Ease::Linear);

        return ret;

    }

    uint8_t Wavepoint::getTransform() const
    {
        return _orientation;
    }


}
