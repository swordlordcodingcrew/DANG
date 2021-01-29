// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#define POCKETMOD_IMPLEMENTATION
#include "pocketmod.h"

#include <iostream>
#include "SndGear.hpp"

namespace dang
{
    bool SndGear::mod_set = false;
    uint8_t SndGear::chan = 1;
    static pocketmod_context context;

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
    uint32_t SndGear::fillWaveBufferWithMod(int16_t *wave_buffer)
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

}
