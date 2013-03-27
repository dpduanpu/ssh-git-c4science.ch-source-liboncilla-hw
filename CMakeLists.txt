project(liboncilla-hw C CXX)
cmake_minimum_required(VERSION 2.8)


find_package(BiorobCMakeUtils REQUIRED)
include_all_biorob_cmake_utils()

make_std_versionning(MAJOR 0 MINOR 0 PATCH 1) 

find_package(PkgConfig REQUIRED)

################################################################################
# Boost, C++11 and CLANG check
################################################################################

find_package(Boost REQUIRED)

if("${CMAKE_CXX_COMPILER}" MATCHES ".*clang.*")
  if( "${Boost_VERSION}" VERSION_LESS "104801")
	message(FATAL_ERROR "There is some incompatibility with boost <= 1.48.0 (here ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}) and C++11 features. Since your compiler (Clang) fully support C++11, you should use a more recent version of Boost to compile this library. C++11 is needed by NemoMath dependency.")
  endif( "${Boost_VERSION}" VERSION_LESS "104801")
endif("${CMAKE_CXX_COMPILER}" MATCHES ".*clang.*")

################################################################################
# Deps
################################################################################

pkg_check_modules(DEPS REQUIRED liboncilla-0 robo-xeno-0.3)

include_directories(${DEPS_INCLUDE_DIRS}
                    ${PROJECT_SOURCE_DIR}/src
                    ${PROJECT_BINARY_DIR}/src)

################################################################################
# Source exploration
################################################################################

add_subdirectory(src)

