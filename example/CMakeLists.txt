set(Obs_EXAMPLES
        obs_example
)

find_package(Jansson MODULE REQUIRED)
find_package(FFmpeg REQUIRED avformat avutil swscale swresample OPTIONAL_COMPONENTS avcodec)
find_package(ZLIB REQUIRED)
find_package(LibUUID REQUIRED)

find_package(X11 REQUIRED)
find_package(X11_XCB MODULE REQUIRED)
# cmake-format: off
find_package(xcb COMPONENTS xcb OPTIONAL_COMPONENTS xcb-xinput QUIET)
# cmake-format: on
#find_package(gio)
find_package(Threads REQUIRED)

add_executable(obs_example obs_example.cpp
        obsimp.cpp
        obsimp.h
        const_define.h)
TARGET_LINK_LIBRARIES(obs_example
        OBS::libobs
        OBS::caption
        OBS::uthash
        FFmpeg::avcodec
        FFmpeg::avformat
        FFmpeg::avutil
        FFmpeg::swscale
        FFmpeg::swresample
        jansson
        ZLIB::ZLIB
        X11::X11_xcb
        xcb
        LibUUID::LibUUID
)



