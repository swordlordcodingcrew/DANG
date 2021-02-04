// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once


#include <cstdint>
#include <vector>

struct pocketmod_context;

namespace dang
{
    class SndGear
    {
    public:
        static void setMod(const uint8_t* mod, const uint32_t len);
        static int16_t setSfx(const uint8_t* sfx, const uint32_t len);
        static uint32_t fillWaveBufferWithMod(int16_t wave_buffer[64]);
        static uint32_t fillWaveBufferWithSfx(int16_t wave_buffer[64], uint8_t ch);

        static uint8_t getMusicChan() { return 0; }

        static bool mod_set;

        struct sfx_struct
        {
            const uint8_t* sfx;
            uint32_t len;
            uint8_t chan;
            uint32_t pos;
            uint32_t loops;
        };

    protected:
        static uint8_t chan;
        static float clip(float value);
        static pocketmod_context context;
        static std::vector<sfx_struct> _sfx_container;

        static uint8_t getSfxChan() { return chan++ % 7 + 1; }
    };

}


