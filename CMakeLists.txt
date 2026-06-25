cmake_minimum_required(VERSION 3.0.0)
project(fft_telecom VERSION 0.1.0)

# include(CTest)
# enable_testing()



if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set(COMPILER_OPTIONS 
        "-std=c++20"
    )
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    set(COMPILER_OPTIONS
        "/std:c++20" "/Yc pch.h"
    )
endif()
    
add_compile_options(
    ${COMPILER_OPTIONS}
)

set(LIBS ${CMAKE_SOURCE_DIR}/libs)
set(LIBS_INCLUDE
    ${LIBS}/TinyRenderer/include
)

add_subdirectory(${LIBS}/TinyRenderer)

add_executable(fft_visualize WIN32 src/main.cpp)
target_include_directories(fft_visualize PRIVATE ${LIBS_INCLUDE})
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    target_link_libraries(fft_visualize "-static" TinyRenderer)
else()
    target_link_libraries(fft_visualize TinyRenderer)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
