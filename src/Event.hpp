// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <cstdint>
#include <functional>

namespace dang
{
    class Event
    {
    public:
        Event();
        explicit Event(int32_t type)  {_type = type; }
        Event(int32_t type, uint16_t filter) {_type = type; _filter = filter;}
        virtual ~Event() = default;

        int32_t     _type{0};
        uint16_t    _filter{0xffff};
    };

}

