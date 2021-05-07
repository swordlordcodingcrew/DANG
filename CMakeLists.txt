cmake_minimum_required(VERSION 3.8)
project(dang)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
        src/dang.hpp
        src/bt/bt.hpp
        src/tween/Tweenable.cpp
        src/tween/Tweenable.hpp
        src/tween/Ease.cpp
        src/tween/Ease.hpp
        src/tween/TwNull.cpp
        src/tween/TwNull.hpp
        src/tween/TwAnim.cpp
        src/tween/TwAnim.hpp
        src/tween/TwPos.cpp
        src/tween/TwPos.hpp
        src/tween/TwVel.cpp
        src/tween/TwVel.hpp
        src/tween/TwAcc.cpp
        src/tween/TwAcc.hpp
        src/tween/TwSequence.cpp
        src/tween/TwSequence.hpp
        src/Imagesheet.cpp
        src/Imagesheet.hpp
        src/Sprite.cpp
        src/Sprite.hpp
        src/Gear.cpp
        src/Gear.hpp
        src/Layer.cpp
        src/Layer.hpp
        src/TileLayer.cpp
        src/TileLayer.hpp
        src/SpriteLayer.cpp
        src/SpriteLayer.hpp
        src/CollisionSpriteLayer.cpp
        src/CollisionSpriteLayer.hpp
        src/BaseHUDLayer.cpp
        src/BaseHUDLayer.hpp
        src/Vector2T.hpp
        src/RectT.hpp
        src/CollisionSprite.cpp
        src/CollisionSprite.hpp
        src/TmxExtruder.cpp
        src/TmxExtruder.hpp
        src/Event.cpp
        src/Event.hpp
        src/Dispatcher.cpp
        src/Dispatcher.hpp
        src/snd/pocketmod.h
        src/snd/SndGear.cpp
        src/snd/SndGear.hpp
        src/snd/wave.h
)

add_library(DANG STATIC ${SOURCES} )

# make sure to link the 32blit supplied stdlibs when compiling for the 32blit HW
# variable pic_stdlibs is set by the 32blit cmake configuration
if(${CMAKE_SYSTEM_NAME} STREQUAL Generic)
    target_link_libraries(DANG ${PIC_STDLIBS})
    set_target_properties(DANG PROPERTIES COMPILE_FLAGS "-fPIC -mno-pic-data-is-text-relative -mno-single-pic-base")
endif()

#target_include_directories(DANG PUBLIC src)
target_include_directories (DANG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

SET(32BLIT_LOC "../32blit-sdk/32blit")
include_directories(32blit-sdk/32blit)

target_include_directories (DANG PUBLIC ${32BLIT_LOC})
#target_include_directories (DANG PUBLIC ${32BLIT_LOC}/audio)
#target_include_directories (DANG PUBLIC ${32BLIT_LOC}/engine)
#target_include_directories (DANG PUBLIC ${32BLIT_LOC}/graphics)
#target_include_directories (DANG PUBLIC ${32BLIT_LOC}/math)
#target_include_directories (DANG PUBLIC ${32BLIT_LOC}/types)

