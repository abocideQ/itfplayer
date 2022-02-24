cmake_minimum_required(VERSION 3.10.2)

# ???.h
include_directories(
        ${CMAKE_HOME_DIRECTORY}
        ${CMAKE_HOME_DIRECTORY}/android
        #ffmpeg
        ${CMAKE_HOME_DIRECTORY}/android_ffmpeg/include
        ${CMAKE_HOME_DIRECTORY}/android_ffmpeg/render
        #opengl
        ${CMAKE_HOME_DIRECTORY}/android_opengles/include
        ${CMAKE_HOME_DIRECTORY}/android_opengles/include/glm
        ${CMAKE_HOME_DIRECTORY}/android_opengles/render
        ${CMAKE_HOME_DIRECTORY}/android_opengles/render_egl
        #opensl
        ${CMAKE_HOME_DIRECTORY}/android_opensles/include
        ${CMAKE_HOME_DIRECTORY}/android_opensles/render
        #itf
        ${CMAKE_HOME_DIRECTORY}/itfcodec
        ${CMAKE_HOME_DIRECTORY}/itfcodec/decode
        ${CMAKE_HOME_DIRECTORY}/itfcodec/encode
        ${CMAKE_HOME_DIRECTORY}/itfcodec/utils
)

# ???.cpp
file(GLOB lib_cpp
        android/*cpp
        #ffmpeg
        android_ffmpeg/include/*.cpp
        android_ffmpeg/render/*.cpp
        #opengl
        android_opengles/include/*.cpp
        android_opengles/include/glm/*.cpp
        android_opengles/render/*.cpp
        android_opengles/egl/*.cpp
        #opensl
        android_opensles/include/*.cpp
        android_opensles/render/*.cpp
        #itf
        itfcodec/*.cpp
        itfcodec/decode/*.cpp
        itfcodec/decode/*.cpp
        itfcodec/utils/*.cpp
        )

# ???.so
add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/android_ffmpeg/include/lib/${ANDROID_ABI}/libavformat.so)
add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/android_ffmpeg/include/lib/${ANDROID_ABI}/libavcodec.so)
add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/android_ffmpeg/include/lib/${ANDROID_ABI}/libavutil.so)
add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/android_ffmpeg/include/lib/${ANDROID_ABI}/libavfilter.so)
add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/android_ffmpeg/include/lib/${ANDROID_ABI}/libswscale.so)
add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/android_ffmpeg/include/lib/${ANDROID_ABI}/libswresample.so)

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

