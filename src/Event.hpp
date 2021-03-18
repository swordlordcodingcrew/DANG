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

        int32_t     _type{0};
        uint16_t    _filter{0xffff};
        bool        _consumed{false};
    };

//    using eventCB = std::bind(void* (dang::Event&), std::placeholders::_1, std::placeholders::_2);
//    std::function<void (dang::Event&)> func = std::bind(&PigsnKings::eventReceived, this, std::placeholders::_1);
}

