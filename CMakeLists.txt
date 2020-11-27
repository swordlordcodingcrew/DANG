cmake_minimum_required(VERSION 3.8)
project(dang)

set(SOURCES
        src/dang.hpp
        src/tween/Tweenable.cpp
        src/tween/Tweenable.h
        src/tween/Ease.cpp
        src/tween/Ease.h
        src/tween/TwAnim.cpp
        src/tween/TwAnim.h
        src/tween/TwPos.cpp
        src/tween/TwPos.h
        src/tween/TwVel.cpp
        src/tween/TwVel.h
        src/tween/TwAcc.cpp
        src/tween/TwAcc.h
        src/tween/TwSequence.cpp
        src/tween/TwSequence.h
        src/Imagesheet.cpp
        src/Imagesheet.h
        src/Sprite.cpp
        src/Sprite.h
        src/Gear.cpp
        src/Gear.h
        src/Layer.cpp
        src/Layer.h
        src/TileLayer.cpp
        src/TileLayer.h
        src/SpriteLayer.cpp
        src/SpriteLayer.h
        src/CollisionSpriteLayer.cpp
        src/CollisionSpriteLayer.h
        src/Vector2T.h
        src/RectT.h
        src/CollisionSprite.cpp
        src/CollisionSprite.h
        src/TmxExtruder.cpp
        src/TmxExtruder.h
        src/Event.cpp
        src/Event.h
        src/Dispatcher.cpp
        src/Dispatcher.h
)

add_library(DANG STATIC ${SOURCES} )

#target_include_directories(DANG PUBLIC src)
target_include_directories (Hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
