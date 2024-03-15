cmake_minimum_required(VERSION 3.10)

project(Maxon)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src app)

add_executable(Active ${app})

target_link_libraries(Active -lEposCmd -lwiringPi)