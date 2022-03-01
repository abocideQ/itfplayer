cmake_minimum_required(VERSION 3.10.2)

# ???.h
include_directories(
        ${CMAKE_HOME_DIRECTORY}
        ${CMAKE_HOME_DIRECTORY}/android
        #ffmpeg
        ${CMAKE_HOME_DIRECTORY}/itf_ffmpeg/include
        ${CMAKE_HOME_DIRECTORY}/itf_ffmpeg/render
        #opengl
        ${CMAKE_HOME_DIRECTORY}/itf_opengles/include
        ${CMAKE_HOME_DIRECTORY}/itf_opengles/include/glm
        ${CMAKE_HOME_DIRECTORY}/itf_opengles/render
        ${CMAKE_HOME_DIRECTORY}/itf_opengles/render_egl
        #opensl
        ${CMAKE_HOME_DIRECTORY}/itf_opensles/include
        ${CMAKE_HOME_DIRECTORY}/itf_opensles/render
        #itf
        ${CMAKE_HOME_DIRECTORY}/itfcodec
)

# ???.cpp
file(GLOB lib_cpp
        android/*cpp
        #ffmpeg
        itf_ffmpeg/include/*.cpp
        itf_ffmpeg/render/*.cpp
        #opengl
        itf_opengles/include/*.cpp
        itf_opengles/include/glm/*.cpp
        itf_opengles/render/*.cpp
        itf_opengles/egl/*.cpp
        #opensl
        itf_opensles/include/*.cpp
        itf_opensles/render/*.cpp
        #itf
        itf_codec/*.cpp
        )

# ???.so
add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/itf_ffmpeg/include/lib/${ANDROID_ABI}/libavformat.so)
add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/itf_ffmpeg/include/lib/${ANDROID_ABI}/libavcodec.so)
add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/itf_ffmpeg/include/lib/${ANDROID_ABI}/libavutil.so)
add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/itf_ffmpeg/include/lib/${ANDROID_ABI}/libavfilter.so)
add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/itf_ffmpeg/include/lib/${ANDROID_ABI}/libswscale.so)
add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/itf_ffmpeg/include/lib/${ANDROID_ABI}/libswresample.so)

set(lib_ffmpeg
        avformat
        avcodec
        avfilter
        avutil
        swscale
        swresample
        )

set(lib_native
        EGL
        GLESv3
        OpenSLES
        log
        android
        )

add_library(lib_itfmedia
        SHARED
        ${lib_cpp}
        )

target_link_libraries(lib_itfmedia
        ${lib_ffmpeg}
        ${lib_native}
        )

