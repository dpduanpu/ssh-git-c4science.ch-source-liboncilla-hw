project(liboncilla-hw C CXX)
cmake_minimum_required(VERSION 2.8)


find_package(BiorobCMakeUtils REQUIRED)
include_all_biorob_cmake_utils()

make_std_versionning(MAJOR 0 MINOR 0 PATCH 1) 

find_package(PkgConfig REQUIRED)


pkg_check_modules(DEPS REQUIRED liboncilla-0)

include_directories(${DEPS_INCLUDE_DIRS})

add_subdirectory(src)




