// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#include "MessageLayer.hpp"

#include <engine/engine.hpp>
#include <engine/input.hpp>
#include <engine/api.hpp>

namespace dang
{

    MessageLayer::MessageLayer() : Layer(Layer::LT_UNDEFINED)
    {
    }

    MessageLayer::MessageLayer(const uint8_t*font) : Layer(Layer::LT_UNDEFINED), _font(font)
    {
    }

    MessageLayer::~MessageLayer()
    {
    }

    void MessageLayer::update(uint32_t dt, const Gear &gear)
    {
        if (blit::buttons.pressed & (_ok | _cancel))
        {
            if (_ttl_cb != nullptr)
            {
                _ttl_cb(blit::buttons.pressed & _ok ? _ok : _cancel);
            }
        }

        if (_ttl > 0 && blit::now() - _start_ms > _ttl)
        {
            if (_ttl_cb != nullptr)
            {
                _ttl_cb(_cancel);
            }
        }
    }

    void MessageLayer::render(const Gear &gear)
    {
        // lovely whitish background
        blit::screen.pen = blit::Pen(255, 255, 255, 150);;
        blit::screen.rectangle(_whitishRect);

        // text in black
        blit::screen.pen = blit::Pen(0, 0, 0, 255);;
        for (size_t i = 0; i < _text.size(); ++i)
        {
            blit::screen.text(_text[i], _font, blit::Point(blit::screen.bounds.w / 2, 30 + i * 10), true, blit::TextAlign::center_h);
        }

    }

    void MessageLayer::setText(const std::string_view& txt)
    {
        _text.clear();

        // split the string_view into vector-entries
        int indexCommaToLeftOfColumn = 0;
        int indexCommaToRightOfColumn = -1;
        for (size_t i = 0; i < txt.size(); ++i)
        {
            if (txt[i] == '\n')
            {
                indexCommaToLeftOfColumn = indexCommaToRightOfColumn;
                indexCommaToRightOfColumn = i;
                int index = indexCommaToLeftOfColumn + 1;
                int length = indexCommaToRightOfColumn - index;

                std::string_view column(txt.data() + index, length);
                _text.push_back(column);
            }
        }
        std::string_view finalColumn(txt.data() + indexCommaToRightOfColumn + 1, txt.size() - indexCommaToRightOfColumn - 1);
        _text.push_back(finalColumn);

        _whitishRect = {34, 26, blit::screen.bounds.w - 68, int32_t(8 + _text.size() * 10) };
    }

    void MessageLayer::setTtl(uint32_t ttl_ms, std::function<void (blit::Button btn)> cb)
    {
        _start_ms = blit::now();
        _ttl = ttl_ms;
        _ttl_cb = cb;
    }

    void MessageLayer::setButtons(blit::Button ok, blit::Button cancel)
    {
        _ok = ok;
        _cancel = cancel;
    }


}