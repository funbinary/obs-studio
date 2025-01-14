cmake_minimum_required(VERSION 3.22...3.25)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

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
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE
            "RelWithDebInfo"
            CACHE STRING "OBS build type [Release, RelWithDebInfo, Debug, MinSizeRel]" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release RelWithDebInfo Debug MinSizeRel)
endif ()

# Global project options
option(ENABLE_HEVC "Enable HEVC encoders" ON)
if (ENABLE_HEVC)
    add_compile_definitions(ENABLE_HEVC)
endif ()
option(BUILD_FOR_DISTRIBUTION "Build for distribution (enables optimizations)" OFF)
option(USE_LIBCXX "Use libc++ instead of libstdc++" ${APPLE})

# todo： remove
option(LINUX_PORTABLE "Build portable version (Linux)" OFF)
option(USE_XDG "Utilize XDG Base Directory Specification (Linux)" ON)
option(ENABLE_PULSEAUDIO "Enable PulseAudio support" ON)
if (OS_LINUX)
    option(ENABLE_WAYLAND "Enable building with support for Wayland (Linux)" ON)
endif ()


setup_obs_project() # define on ObsDefaults_Linux


# OBS sources and plugins
add_subdirectory(deps)

add_subdirectory(libobs)
add_subdirectory(libobs-opengl)

add_subdirectory(plugins)


add_subdirectory(example)