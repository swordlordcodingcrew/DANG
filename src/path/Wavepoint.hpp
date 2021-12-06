// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <src/Vector2T.hpp>
#include <cstdint>
#include <memory>

namespace dang
{
//    class TmxExtruder;
    class TwPos;
    using spTwPos = std::shared_ptr<TwPos>;

    class Wavepoint
    {
    public:
        Wavepoint() = default;
        Wavepoint(uint32_t id, Vector2F pos, uint32_t next_id, uint32_t duration, uint8_t orientation, uint32_t delay);

        spTwPos     getTween() const;
        uint8_t     getTransform() const;
        bool        endPoint() const { return _next == nullptr; }
        const Wavepoint* next() const { return _next; };
        uint32_t    id() const { return _id; }

    protected:
        friend class TmxExtruder;
        Wavepoint*    _next{nullptr};
        uint32_t      _next_id{0};
        uint32_t      _id{0};
        uint32_t      _duration{0};

        // flip possibilities in 32blit
        //       NONE = 0b000,          0
        //       HORIZONTAL = 0b001,    1
        //       VERTICAL = 0b010,      2
        //       XYSWAP = 0b100,        4
        //       R90 = 0b101,           5
        //       R180 = 0b011,          3
        //       R270 = 0b110           6
        uint8_t        _orientation{0};
        uint32_t       _delay{0};
        Vector2F       _pos{0,0};

    };

}

