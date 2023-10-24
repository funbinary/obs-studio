cmake_minimum_required(VERSION 3.22...3.25)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")
include(VersionConfig)


project(obs-studio VERSION ${OBS_VERSION_CANONICAL})
# 控制是否使用目标属性将 FOLDER 目标组织到文件夹中。顶级 USE_FOLDERS CMakeLists.txt 文件末尾的值决定了行为。
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 定义函数：旧的， 没啥作用
include(ObsHelpers)
# Set default compiler flags
include(CompilerConfig)

# Allow selection of common build types via UI
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "RelWithDebInfo"
      CACHE STRING "OBS build type [Release, RelWithDebInfo, Debug, MinSizeRel]" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release RelWithDebInfo Debug MinSizeRel)
endif()

# Global project options
option(ENABLE_HEVC "Enable HEVC encoders" ON)
if(ENABLE_HEVC)
  add_compile_definitions(ENABLE_HEVC)
endif()
option(BUILD_FOR_DISTRIBUTION "Build for distribution (enables optimizations)" OFF)
option(ENABLE_UI "Enable building with UI (requires Qt)" ON)
option(ENABLE_SCRIPTING "Enable scripting support" ON)
option(USE_LIBCXX "Use libc++ instead of libstdc++" ${APPLE})
option(BUILD_TESTS "Build test directory (includes test sources and possibly a platform test executable)" OFF)

if(OS_WINDOWS)
  option(INSTALLER_RUN
         "Build a multiarch installer (needs to run independently after both archs have compiled) (Windows)" OFF)

elseif(OS_POSIX)
  option(LINUX_PORTABLE "Build portable version (Linux)" OFF)
  option(USE_XDG "Utilize XDG Base Directory Specification (Linux)" ON)
  option(ENABLE_PULSEAUDIO "Enable PulseAudio support" ON)
  if(OS_LINUX)
    option(ENABLE_WAYLAND "Enable building with support for Wayland (Linux)" ON)
    option(BUILD_FOR_PPA "Build for PPA distribution" OFF)
  endif()
endif()

setup_obs_project()
mark_as_advanced(BUILD_TESTS USE_LIBCXX)

if(INSTALLER_RUN)
  generate_multiarch_installer()
  return()
endif()

# OBS sources and plugins
add_subdirectory(deps)

add_subdirectory(libobs)
add_subdirectory(plugins)


# Tests
if(ENABLE_UNIT_TESTS)
  enable_testing()
endif()

if(BUILD_TESTS OR ENABLE_UNIT_TESTS)
  add_subdirectory(test)
endif()
add_subdirectory(example)