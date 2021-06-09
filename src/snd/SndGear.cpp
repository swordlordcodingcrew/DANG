// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#define POCKETMOD_IMPLEMENTATION
#include "pocketmod.h"

#include <audio/audio.hpp>
#include <cassert>
#include <src/dang.hpp>
#include "SndGear.hpp"
#include "wave.h"

namespace dang
{
    bool SndGear::mod_set = false;
    bool SndGear::xm_set = false;
    uint8_t SndGear::chan = 0;
    pocketmod_context SndGear::mod_ctx;
    xm_context_t* SndGear::xm_ctx;
    std::vector<SndGear::sfx_struct> SndGear::_sfx_container(7, {nullptr, 0, 0, 0, 0});


    uint8_t SndGear::playSfx(const uint8_t *sfx, const uint32_t len, float volume)
    {
        if (volume > 1) volume = 1;
        if (volume < 0) volume = 0;

        const wave_header* wh = (wave_header*)sfx;

        // check format
        assert(wh->format_type == 1);
        // wav must be mono
        assert(wh->channels == 1);
        // sample must be 22050
        assert(wh->sample_rate == 22050);

        uint8_t ch = getSfxChan();
        SndGear::_sfx_container.at(ch-1).chan = ch;
        SndGear::_sfx_container.at(ch-1).len = wh->data_size / (wh->bits_per_sample / 8) + sizeof(wave_header);
        SndGear::_sfx_container.at(ch-1).sfx = sfx;
        SndGear::_sfx_container.at(ch-1).pos = sizeof(wave_header);
        SndGear::_sfx_container.at(ch-1).loops = 0;

        blit::channels[ch].waveforms = blit::Waveform::WAVE; // Set type to WAVE
        blit::channels[ch].wave_buffer_callback = &SndGear::sfx_buff_cb;  // Set callback address
        blit::channels[ch].volume = volume * 0xffff;
        blit::channels[ch].user_data = (void*)ch;
        blit::channels[ch].trigger_attack();

        return ch;
    }

    void SndGear::playMod(const uint8_t* mod, const uint32_t len, float volume)
    {
        if (xm_set)
        {
            return;
        }

        if (volume > 1) volume = 1;
        if (volume < 0) volume = 0;

        int32_t result = pocketmod_init(&mod_ctx, (void *) mod, len, 11025);

        if (result == 0)
        {
            mod_set = false;
            D_DEBUG_PRINT("the data is not recognised as a module.")
        }
        else
        {
            mod_set = true;
            blit::channels[dang::SndGear::getMusicChan()].waveforms = blit::Waveform::WAVE; // Set type to WAVE
            blit::channels[dang::SndGear::getMusicChan()].wave_buffer_callback = &SndGear::mod_buff_cb;  // Set callback address
            blit::channels[dang::SndGear::getMusicChan()].volume = volume * 0xffff;
            blit::channels[dang::SndGear::getMusicChan()].trigger_attack();
        }
    }

    void SndGear::playXM(const uint8_t *mod, uint32_t len, float volume)
    {
        if (mod_set)
        {
            return;
        }

        if (volume > 1) volume = 1;
        if (volume < 0) volume = 0;

        releaseXM();

        xm_create_context(&xm_ctx, (const char*)mod, 11025);

        if (xm_ctx == nullptr)
        {
            xm_set = false;
            D_DEBUG_PRINT("the data is not recognised as an xm module.");
        }
        else
        {
            xm_set = true;
            blit::channels[dang::SndGear::getMusicChan()].waveforms = blit::Waveform::WAVE; // Set type to WAVE
            blit::channels[dang::SndGear::getMusicChan()].wave_buffer_callback = &SndGear::xm_buff_cb;  // Set callback address
            blit::channels[dang::SndGear::getMusicChan()].volume = volume * 0xffff;
            blit::channels[dang::SndGear::getMusicChan()].trigger_attack();
        }
    }

    void SndGear::sfx_buff_cb(blit::AudioChannel &channel)
    {
        uint8_t nr = (uint64_t)channel.user_data;
        sfx_struct& sfxs = SndGear::_sfx_container.at(nr-1);

        // Copy 64 bytes to the channel audio buffer
        for (int x = 0; x < 64; x++)
        {
            // If current sample position is greater than the sample length, fill the rest of the buffer with zeros.
            int16_t val = ((int16_t*)sfxs.sfx)[sfxs.pos];
            int16_t sample = sfxs.pos < sfxs.len ? (sfxs.sfx[sfxs.pos] << 8) - 0x7f00 : 0;
            channel.wave_buffer[x] = sfxs.pos < sfxs.len ? val : 0;

            sfxs.pos++;
        }

        if (sfxs.pos >= sfxs.len)
        {
//            sfxs.loops++;
            channel.off();        // Stop playback of this channel.
            channel.waveforms = 0;
            channel.wave_buffer_callback = nullptr;
            channel.volume = 0x7fff;
            channel.user_data = nullptr;

        }

    }


    void SndGear::mod_buff_cb(blit::AudioChannel &channel)
    {
        if (mod_set)
        {
            float buffer[32][2];
            int16_t output1, output2;

            /* Render a chunk of samples */
            int rendered_bytes = pocketmod_render(&mod_ctx, buffer, sizeof(buffer));

            /* Convert the sample data to 16-bit and write it to the buffer */
            for (int i = 0; i < 32; i++)
            {
                output1 = (int16_t) (clip(buffer[i][0]) * 0x7fff);
                output2 = (int16_t) (clip(buffer[i][1]) * 0x7fff);
                channel.wave_buffer[2*i] = output2;
                channel.wave_buffer[2*i + 1] = output1;
            }

            if (pocketmod_loop_count(&mod_ctx) > 0)
            {
                stopMod();
            };
        }
        else
        {
            stopMod();
        }
    }

    void SndGear::xm_buff_cb(blit::AudioChannel &channel)
    {
        if (xm_set)
        {
            float buffer[64];
            int16_t output1, output2;
            uint32_t loop_count = xm_get_loop_count(xm_ctx);

            if (loop_count < 1)
            {
                xm_generate_samples(xm_ctx, buffer, 32);

                /** Convert the sample data to 16-bit and write it to the buffer */
                for (int i = 0; i < 32; i++)
                {
                    output1 = (int16_t) (clip(buffer[2*i]) * 0x7fff);
                    output2 = (int16_t) (clip(buffer[2*i+1]) * 0x7fff);
                    channel.wave_buffer[2*i] = output1;
                    channel.wave_buffer[2*i + 1] = output2;
                }
            }
            else
            {
                stopXM();
            }
        }
        else
        {
            stopXM();
        }
    }

    void SndGear::stopMod()
    {
        blit::channels[dang::SndGear::getMusicChan()].off();
        blit::channels[dang::SndGear::getMusicChan()].wave_buffer_callback = nullptr;
    }

    void SndGear::stopXM()
    {
        blit::channels[dang::SndGear::getMusicChan()].off();
        blit::channels[dang::SndGear::getMusicChan()].wave_buffer_callback = nullptr;
        dang::SndGear::releaseXM();
    }

    void SndGear::releaseXM()
    {
        if (xm_set)
        {
            xm_free_context(xm_ctx);
            xm_set = false;
        }
    }

    /* Clip a floating point sample to the [-1, +1] range */
    float SndGear::clip(float value)
    {
        value = value < -1.0f ? -1.0f : value;
        value = value > +1.0f ? +1.0f : value;
        return value;
    }


}
