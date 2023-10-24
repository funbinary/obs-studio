cmake_minimum_required(VERSION 3.22...3.25)

option(ENABLE_PLUGINS "Enable building OBS plugins" ON)

if(NOT ENABLE_PLUGINS)
  obs_status(STATUS "Building with plugins disabled.")
  return()
endif()


#  -DENABLE_NEW_MPEGTS_OUTPUT=OFF
# APPLE/WIN32/UNIX are soft-deprecated: https://discourse.cmake.org/t/platform-id-vs-win32-vs-cmake-system-name/1226/2
if(OS_LINUX)
  add_subdirectory(linux-capture)
  add_subdirectory(linux-pulseaudio)
  add_subdirectory(linux-v4l2)
  add_subdirectory(linux-jack)
  add_subdirectory(linux-alsa)
endif()

add_subdirectory(image-source)
add_subdirectory(obs-x264)
add_subdirectory(obs-libfdk)
add_subdirectory(obs-ffmpeg)
add_subdirectory(obs-outputs)
add_subdirectory(obs-filters)
add_subdirectory(obs-transitions)
add_subdirectory(rtmp-services)
add_subdirectory(obs-webrtc)
