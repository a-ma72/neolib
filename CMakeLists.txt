cmake_minimum_required(VERSION 3.14)
set(CMAKE_VERBOSE_MAKEFILE OFF)

add_library(neolib INTERFACE)
target_include_directories(neolib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_definitions(neolib -DDONT_HAVE_ALLOCATOR_TRAITS)
