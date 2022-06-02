cmake_minimum_required(VERSION 3.3.2)

project(masterofpuppets)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

file(GLOB_RECURSE masterofpuppets_SOURCES "src/*.cpp")
file(GLOB_RECURSE masterofpuppets_HEADERS "src/*.h")

foreach (_headerFile ${masterofpuppets_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND masterofpuppets_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES masterofpuppets_INCLUDE_DIRS)

configure_file("src/zoomout.pov" "zoomout.pov" COPYONLY)
configure_file("src/topdown.pov" "topdown.pov" COPYONLY)
configure_file("src/closein.pov" "closein.pov" COPYONLY)

add_executable(masterofpuppets ${masterofpuppets_SOURCES})

install(TARGETS masterofpuppets DESTINATION ~/bin)
