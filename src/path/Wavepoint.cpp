// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "Wavepoint.hpp"
#include "../tween/TwPos.hpp"

namespace dang
{
    Wavepoint::Wavepoint(uint32_t id, dang::Vector2F pos, uint32_t next_id, uint32_t duration, uint8_t orientation, uint32_t delay)
    : _id(id), _pos(pos), _next_id(next_id), _duration(duration), _orientation(orientation), _delay(delay)
    {
        _next = nullptr;
    }

    spTwPos Wavepoint::getTween() const
    {
        if (_next == nullptr)
        {
            return nullptr;
        }

        dang::spTwPos ret = std::make_shared<TwPos>(_next->_pos, _duration, Ease::Linear);

        return ret;
    }

    uint8_t Wavepoint::getTransform() const
    {
        return _orientation;
    }

}
