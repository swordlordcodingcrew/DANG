// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once


#include <cstdint>
#include <vector>
#include "32blit.hpp"

struct pocketmod_context;

namespace dang
{
    class SndGear
    {
    public:
        static uint8_t playSfx(const uint8_t* sfx, const uint32_t len, float volume);
        static void playMod(const uint8_t* mod, const uint32_t len, float volume);
        static void stopMod();
        static void changeModVolume(float volume);

        static void playRumble(const uint8_t len);
        static void updateRumble();

    protected:

        static void sfx_buff_cb(blit::AudioChannel &channel);
        static void mod_buff_cb(blit::AudioChannel &channel);

        static uint8_t getMusicChan() { return 0; }

        static bool mod_set;

        static uint8_t _rumbleLen;

        struct sfx_struct
        {
            const uint8_t* sfx;
            uint32_t len;
            uint8_t chan;
            uint32_t pos;
            uint32_t loops;
        };

    protected:
        /** mod stuff */
        static float clip(float value);
        static pocketmod_context mod_ctx;

        /** sfx */
        static std::vector<sfx_struct> _sfx_container;
        static uint8_t chan;
        // round robin
        static uint8_t getSfxChan() { return chan++ % 7 + 1; }
    };

}


