project(TinyRenderer VERSION 0.0.1)

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

file(GLOB TINY_RENDERER_SRC_FILES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/*.cpp"
)
add_library(TinyRenderer STATIC ${TINY_RENDERER_SRC_FILES})

target_include_directories(TinyRenderer PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(TinyRenderer PRIVATE ${PROJECT_SOURCE_DIR}/include/../)