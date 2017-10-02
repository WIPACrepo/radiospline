cmake_minimum_required(VERSION 2.8.12)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_C_STANDARD 99)

# Piggyback on photospline's library-finding
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../photospline/cmake/Packages/")
include(CFITSIO)
include(GSL)
include(LAPACK)

if(APPLE)
  # See: https://cmake.org/Wiki/CMake_RPATH_handling
  set(CMAKE_MACOSX_RPATH TRUE)
  set(CMAKE_SKIP_BUILD_RPATH FALSE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif(APPLE)

find_library(PHOTOSPLINE_LIB photospline PATHS ${CMAKE_INSTALL_DIR}/lib)
find_path(PHOTOSPLINE_INCLUDE_DIR photospline/splinetable.h PATHS ${CMAKE_INSTALL_DIR}/include)

# The primary library for interfacing to the spline raytrace tables
add_library(radiospline
    SHARED
	private/lib/FirnShadow.cxx
    private/lib/RayTrace.cxx
    private/lib/IceGeometry.cxx
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/public
                    ${PHOTOSPLINE_INCLUDE_DIR})

target_link_libraries(radiospline LINK_PUBLIC 
                                  ${LAPACK_LIBRARIES} ${CFITSIO_LIBRARIES} ${GSL_LIBRARIES}
                                  ${PHOTOSPLINE_LIB})

add_executable(ray private/test/ray.cxx)
target_link_libraries(ray LINK_PUBLIC radiospline)

add_executable(firn private/test/firn.cxx)
target_link_libraries(firn LINK_PUBLIC radiospline)

add_executable(raybench private/test/raybench.cxx)
target_link_libraries(raybench LINK_PUBLIC radiospline)

install(TARGETS firn ray raybench radiospline
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)
