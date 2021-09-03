// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

/*
 * Install Packages:
- sudo apt install ffmpeg
- sudo apt install libavutil-dev
- sudo apt install libavformat-dev
- sudo apt install libswscale-dev
 */

#pragma once

#define DANG_DEBUG_PRINT

#ifdef DANG_DEBUG_PRINT
#ifdef TARGET_32BLIT_HW
        #define D_DEBUG_PRINT(...) blit::debugf(__VA_ARGS__)
    #else
        #define D_DEBUG_PRINT(...) std::printf(__VA_ARGS__)
    #endif
#else
#define D_DEBUG_PRINT(...)
#endif

//#define DANG_DEBUG
//#define DANG_DEBUG_DRAW


