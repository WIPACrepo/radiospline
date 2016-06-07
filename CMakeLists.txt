cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/") 

# Superbuild CMake file for radiospline and photospline library
# J. Kelley
# 
# TO DO
# - clean up find_library in radiospline CMakeLists
# - figure out why library source dependencies are broken (make doesn't
#   detect changes)
# - switch back to CPC when source is updated
# - move tables into git
#

# Get the photospline project from SVN
include(ExternalProject)
ExternalProject_Add(photospline
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/photospline
  DOWNLOAD_NAME aepk_v1_1.tar.gz
  #URL http://cpc.cs.qub.ac.uk/cpc/cgi-bin/processrequest.pl/?catid=aepk_v1_0
  URL http://icecube.wisc.edu/~jkelley/ara/aepk_v1_1.tar.gz
  UPDATE_COMMAND ""
  INSTALL_DIR ${CMAKE_BINARY_DIR}/install
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)

# Grab the tables from the web
ExternalProject_Add(radiospline_tables
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources/tables/exponential
    URL http://icecube.wisc.edu/~jkelley/ara/icetables_exponential_160414.tar.gz
    URL_MD5 428cb6bf44b7394513fcca7983c6e97b
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    INSTALL_DIR .
)

ExternalProject_Add(radiospline
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src
    DEPENDS photospline
    INSTALL_DIR ${CMAKE_BINARY_DIR}/install
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)


