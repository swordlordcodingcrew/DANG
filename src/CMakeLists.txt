list(APPEND SOURCES
        dang.hpp
        tween/Tweenable.cpp
        tween/Tweenable.h
        tween/Ease.cpp
        tween/Ease.h
        tween/TwAnim.cpp
        tween/TwAnim.h
        tween/TwPos.cpp
        tween/TwPos.h
        tween/TwVel.cpp
        tween/TwVel.h
        tween/TwAcc.cpp
        tween/TwAcc.h
        tween/TwSequence.cpp
        tween/TwSequence.h
        Imagesheet.cpp
        Imagesheet.h
        Sprite.cpp
        Sprite.h
        Gear.cpp
        Gear.h
        Layer.cpp
        Layer.h
        TileLayer.cpp
        TileLayer.h
        SpriteLayer.cpp
        SpriteLayer.h
        CollisionSpriteLayer.cpp
        CollisionSpriteLayer.h
        Vector2T.h
        RectT.h
        CollisionSprite.cpp
        CollisionSprite.h
        TmxExtruder.cpp
        TmxExtruder.h
        Event.cpp
        Event.h
        Dispatcher.cpp
        Dispatcher.h
)

add_library(DANG STATIC ${SOURCES} )

target_include_directories(DANG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
