list(APPEND SOURCES
    dang_globals.hpp
    dang_collision.hpp
    tween/Tweenable.cpp
    tween/Tweenable.h
    tween/Ease.cpp
    tween/Ease.h
    tween/TwAnim.cpp
    tween/TwAnim.h
    tween/TwMove.cpp
    tween/TwMove.h
    Imagesheet.cpp
    Imagesheet.h
)

add_library(DANG STATIC ${SOURCES})

target_include_directories(DANG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
