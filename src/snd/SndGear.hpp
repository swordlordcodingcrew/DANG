// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once


#include <cstdint>

struct pocketmod_context;

namespace dang
{
    class SndGear
    {
    public:
        static void setMod(const uint8_t* mod, const uint32_t len);
        static uint32_t fillWaveBufferWithMod(int16_t* wave_buffer);

        static uint8_t getMusicChan() { return 0; }
        static uint8_t getSfxChan() { return ++chan % 15 + 1; }

    protected:
        static bool mod_set;
        static uint8_t chan;
        static float clip(float value);
//        static pocketmod_context context;
    };

}


