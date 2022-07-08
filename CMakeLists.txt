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
        src/Gear.cpp
        src/Gear.hpp
        src/layer/Layer.cpp
        src/layer/Layer.hpp
        src/layer/TileLayer.cpp
        src/layer/TileLayer.hpp
        src/layer/SimpleImageLayer.cpp
        src/layer/SimpleImageLayer.hpp
        src/layer/MessageLayer.cpp
        src/layer/MessageLayer.hpp
        src/layer/BaseHUDLayer.cpp
        src/layer/BaseHUDLayer.hpp
        src/Vector2T.hpp
        src/RectT.hpp
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
        src/tween/TwPosSpline.cpp
        src/tween/TwPosSpline.h
        src/sprite/CollisionObject.cpp
        src/sprite/CollisionObject.hpp
        src/CollisionSolver.cpp
        src/CollisionSolver.hpp
        src/sprite/SpriteObject.cpp
        src/sprite/SpriteObject.hpp
        src/sprite/ImageObject.cpp
        src/sprite/ImageObject.hpp
        src/sprite/MotionObject.cpp
        src/sprite/MotionObject.hpp
        src/sprite/ColSpr.cpp
        src/sprite/ColSpr.hpp
        src/sprite/FullColSpr.cpp
        src/sprite/FullColSpr.hpp
        src/layer/ColSprLayer.cpp
        src/layer/ColSprLayer.hpp
        src/layer/ImgSprLayer.cpp
        src/layer/ImgSprLayer.hpp
        src/SprIterator.cpp
        src/SprIterator.hpp
        src/sprite/ImgSpr.cpp
        src/sprite/ImgSpr.hpp
        src/layer/SprLayer.cpp
        src/layer/SprLayer.hpp
        src/sprite/FullImgSpr.cpp
        src/sprite/FullImgSpr.hpp
        src/sprite/TweenObject.cpp
        src/sprite/TweenObject.hpp)

add_library(DANG STATIC ${SOURCES} )

# make sure to link the 32blit supplied stdlibs when compiling for the 32blit HW
# variable pic_stdlibs is set by the 32blit cmake configuration
if(${CMAKE_SYSTEM_NAME} STREQUAL Generic)
    target_link_libraries(DANG ${PIC_STDLIBS})
    set_target_properties(DANG PROPERTIES COMPILE_FLAGS "-fPIC -mno-pic-data-is-text-relative -mno-single-pic-base")
endif()

target_include_directories (DANG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (DANG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

SET(32BLIT_LOC "../32blit-sdk/32blit")
include_directories(32blit-sdk/32blit)

target_include_directories (DANG PUBLIC ${32BLIT_LOC})
#target_include_directories (DANG PUBLIC ${32BLIT_LOC}/audio)
#target_include_directories (DANG PUBLIC ${32BLIT_LOC}/engine)
#target_include_directories (DANG PUBLIC ${32BLIT_LOC}/graphics)
#target_include_directories (DANG PUBLIC ${32BLIT_LOC}/math)
#target_include_directories (DANG PUBLIC ${32BLIT_LOC}/types)

