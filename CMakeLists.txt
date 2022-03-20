cmake_minimum_required(VERSION 3.8)
project(dang)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED True)

# if you want to see dang debug info drawn, enable this
#add_definitions(-DDANG_DEBUG_PRINT)
#add_definitions(-DDANG_DEBUG_DRAW)

add_compile_options("-Wextra" "-Wdouble-promotion" "-fexceptions" "-Wattributes")

set(SOURCES
        src/dang.hpp
        src/path/SceneGraph.cpp
        src/path/SceneGraph.hpp
        src/path/Waypoint.cpp
        src/path/Waypoint.hpp
        src/bulletml/BMLNode.cpp
        src/bulletml/BMLNode.h
        src/bulletml/BMLState.cpp
        src/bulletml/BMLState.h
        src/bulletml/BMLTree.cpp
        src/bulletml/BMLTree.h
        src/bulletml/BMLBuilder.cpp
        src/bulletml/BMLBuilder.h
        src/bt/BTNode.cpp
        src/bt/BTNode.h
        src/bt/NTree.cpp
        src/bt/NTree.h
        src/bt/NTreeState.cpp
        src/bt/NTreeState.h
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
        src/tween/TwAccX.cpp
        src/tween/TwAccY.cpp
        src/tween/TwAccY.hpp
        src/tween/TwVelY.cpp
        src/tween/TwVelY.hpp
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
        src/SimpleImageLayer.cpp
        src/SimpleImageLayer.hpp
        src/MessageLayer.cpp
        src/MessageLayer.hpp
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
        src/bt/NTBuilder.cpp
        src/bt/NTBuilder.h
        src/DangFwdDecl.h
        src/Rand.cpp
        src/Rand.hpp
        src/path/Wavepoint.cpp
        src/path/Wavepoint.hpp
        src/SpriteIterator.cpp
        src/SpriteIterator.hpp
        src/tween/TwPosSpline.cpp
        src/tween/TwPosSpline.h src/CollisionObject.cpp src/CollisionObject.hpp src/CollisionSolver.cpp src/CollisionSolver.hpp)

add_library(DANG STATIC ${SOURCES} )
#target_link_libraries(DANG xmp_lite)

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

