// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#define POCKETMOD_IMPLEMENTATION
#include "pocketmod.h"

#include <iostream>
#include "SndGear.hpp"
#include "wave.h"

namespace dang
{
    bool SndGear::mod_set = false;
    uint8_t SndGear::chan = 0;
    pocketmod_context SndGear::context;
    std::vector<SndGear::sfx_struct> SndGear::_sfx_container(7, {nullptr, 0, 0, 0, 0});

    void SndGear::setMod(const uint8_t *mod, const uint32_t len)
    {
        int32_t result = pocketmod_init(&context, (void *) mod, len, 11025);

        if (result == 0)
        {
            mod_set = false;
            std::cout << "the data is not recognised as a module." << std::endl;
        }
        else
        {
            mod_set = true;
        }

    }

    /* Clip a floating point sample to the [-1, +1] range */
    float SndGear::clip(float value)
    {
        value = value < -1.0f ? -1.0f : value;
        value = value > +1.0f ? +1.0f : value;
        return value;
    }

    /**
     * This function fills up the wave_buffer of the Audochannel struct. Be aware, no checks are done. The array-size has to be 64
     * @param wave_buffer
     * @return loop count of mod
     */
    uint32_t SndGear::fillWaveBufferWithMod(int16_t wave_buffer[64])
    {
        if (mod_set)
        {
            float buffer[32][2];
            int16_t output1, output2;

            /* Render a chunk of samples */
            int rendered_bytes = pocketmod_render(&context, buffer, sizeof(buffer));
//            int rendered_samples = rendered_bytes / sizeof(float[2]);

            /* Convert the sample data to 16-bit and write it to the buffer */
            for (int i = 0; i < 32; i++)
            {
                output1 = (int16_t) (clip(buffer[i][0]) * 0x7fff);
                output2 = (int16_t) (clip(buffer[i][1]) * 0x7fff);
                wave_buffer[2*i] = output2;
                wave_buffer[2*i + 1] = output1;
            }

            return pocketmod_loop_count(&context);
        }

        return 0;
    }

    uint32_t SndGear::fillWaveBufferWithSfx(int16_t wave_buffer[64], uint8_t ch)
    {
//        sfx_struct& sfxs = SndGear::_sfx_container[ch-1];
        sfx_struct& sfxs = SndGear::_sfx_container.at(ch-1);

        // Copy 64 bytes to the channel audio buffer
        for (int x = 0; x < 64; x++)
        {
            // If current sample position is greater than the sample length, fill the rest of the buffer with zeros.
            int16_t val = ((int16_t*)sfxs.sfx)[sfxs.pos];
            int16_t sample = sfxs.pos < sfxs.len ? (sfxs.sfx[sfxs.pos] << 8) - 0x7f00 : 0;
//            wave_buffer[x] = sfxs.pos < sfxs.len ? (sfxs.sfx[sfxs.pos] << 8) - 0x7f00 : 0;
            wave_buffer[x] = sfxs.pos < sfxs.len ? val : 0;

            sfxs.pos++;
        }

        if (sfxs.pos >= sfxs.len)
        {
            sfxs.loops++;
        }

        return sfxs.loops;
    }

    int16_t SndGear::setSfx(const uint8_t *sfx, const uint32_t len)
    {
        const wave_header* wh = (wave_header*)sfx;

        if (wh->format_type != 1)
        {
            std::cout << "unsupported format ( format type " << wh->format_type << ")" << std::endl;
            return -1;
        }

        if (wh->channels != 1)
        {
            std::cout << "wave is stereo, but should be mono" << std::endl;
            return -1;
        }

        if (wh->sample_rate != 22050)
        {
            std::cout << "sample rate is not 22050" << std::endl;
            return -1;
        }

        uint8_t ch = getSfxChan();
        SndGear::_sfx_container.at(ch-1).chan = ch;
        SndGear::_sfx_container.at(ch-1).len = wh->data_size / (wh->bits_per_sample / 8) + sizeof(wave_header);
        SndGear::_sfx_container.at(ch-1).sfx = sfx;
        SndGear::_sfx_container.at(ch-1).pos = sizeof(wave_header);
        SndGear::_sfx_container.at(ch-1).loops = 0;

        return ch;
    }

}
