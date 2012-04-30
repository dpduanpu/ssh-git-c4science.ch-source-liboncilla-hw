project(liboncilla-hw C CXX)
cmake_minimum_required(VERSION 2.8)



set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(PATCH_VERSION 0)

set(API_VERSION ${MAJOR_VERSION}.${MINOR_VERSION})

if(${MAJOR_VERSION} EQUAL 0)
  set(ABI_VERSION ${API_VERSION})
else(${MAJOR_VERSION} EQUAL 0)
  set(ABI_VERSION ${MAJOR_VERSION})
endif(${MAJOR_VERSION} EQUAL 0)

set(VERSION_STRING ${API_VERSION}.${PATCH_VERSION})


find_package(PkgConfig REQUIRED)


pkg_check_modules(LIBSBCP libsbcp-0 REQUIRED)
pkg_check_modules(BIOROB_RBIO biorob-rbio-0 REQUIRED)

include_directories(${LIBSBCP_INCLUDE_DIRS} ${BIOROB_RBIO_INCLUDE_DIRS})

add_subdirectory(src)




