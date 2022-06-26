// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once

#include "src/layer/Layer.hpp"

#include <types/rect.hpp>
#include <graphics/font.hpp>

#include <vector>
#include <functional>
#include <engine/input.hpp>

namespace dang
{
    class MessageLayer : public Layer
    {
    public:
        explicit MessageLayer(const uint8_t * font);
        MessageLayer(const uint8_t * font, const PointF& position, uint8_t z_order, const std::string& name, bool visible, bool active);
        ~MessageLayer() override = default;

        void    update(uint32_t dt, const Gear& gear) override;
        void    render(const Gear& gear) override;

        void    setText(const std::string_view& txt);
        void    setTtl(uint32_t ttl_ms, std::function<void (blit::Button btn)> cb);
        void    setButtons(blit::Button ok, blit::Button cancel);

    protected:
        MessageLayer();

        std::vector<std::string_view> _text;
        const uint8_t*    _font{nullptr};
        blit::Rect _whitishRect{0,0,0,0};

        uint32_t _ttl{0};
        std::function<void (blit::Button)> _ttl_cb{nullptr};

        uint32_t _start_ms{0};

        blit::Button _ok{blit::Button::X};
        blit::Button _cancel{blit::Button::Y};
    };

}

