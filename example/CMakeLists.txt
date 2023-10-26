set(Obs_EXAMPLES
        obs_example
)
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/release/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/release/bin)
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

add_executable(obs_example)
add_executable(OBS::example ALIAS obs_example)

target_sources(obs_example PRIVATE obs_example.cpp
        obsimp.cpp
        obsimp.h
        const_define.h)

target_link_libraries(obs_example
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

setup_binary_target(obs_example)

