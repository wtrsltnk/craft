cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
project(craft)
set(VERSION "1.0.0")

add_subdirectory(libzynth/src)

include_directories(
    libzynth/src
    ${FFTW_INCLUDE_DIR}
    ${MXML_INCLUDE_DIR}
    ${PTHREAD_INCLUDE_DIR}
    )

add_subdirectory(whoosh)
add_subdirectory(bam)
add_subdirectory(midi-recorder)
