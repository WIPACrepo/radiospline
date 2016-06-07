cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/Modules/")
 
find_library(BLAS blas goto2
             PATHS $ENV{I3_PORTS}/lib)
find_library(CFITSIO cfitsio
             PATHS $ENV{I3_PORTS}/lib)
find_path(CFITSIO_INCLUDE_DIR fitsio.h
          PATHS $ENV{I3_PORTS}/include)
find_package(GSL REQUIRED)

# FIX ME THIS IS SHITE
find_library(PHOTOSPLINE_LIB photospline PATHS
             ${CMAKE_BINARY_DIR}/../../../photospline-prefix/src/photospline-build)
message("Looking in ${CMAKE_BINARY_DIR}/photospline-prefix/src/photospline-build")
find_path(PHOTOSPLINE_INCLUDE_DIR photospline/splinetable.h
          PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../photospline/include)

# The primary library for interfacing to the spline raytrace tables
add_library(radiospline
    SHARED
    private/lib/SplineTable.cxx
	private/lib/FirnShadow.cxx
    private/lib/RayDelay.cxx
    private/lib/IceGeometry.cxx
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/public
                    ${PHOTOSPLINE_INCLUDE_DIR})

target_link_libraries(radiospline LINK_PUBLIC 
                                  ${BLAS} ${CFITSIO} ${GSL_LIBRARIES}                                  
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
