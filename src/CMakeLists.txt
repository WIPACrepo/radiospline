cmake_minimum_required(VERSION 2.8.12)

# Piggyback on photospline's library-finding
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../photospline/cmake/Packages/")
include(CFITSIO)
include(GSL)
include(LAPACK)

find_library(PHOTOSPLINE_LIB photospline PATHS ${CMAKE_INSTALL_DIR}/lib)
find_path(PHOTOSPLINE_INCLUDE_DIR photospline/splinetable.h PATHS ${CMAKE_INSTALL_DIR}/include)

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
