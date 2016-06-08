cmake_minimum_required(VERSION 2.8.12)

# Superbuild CMake file for radiospline and photospline library
# J. Kelley
# 
# TO DO
# - figure out how to pass environment to photospline cmake
# - switch back to CPC when source is updated
# - move tables into git
#

# Get the photospline project from SVN
set(photospline_url "http://icecube.wisc.edu/~jkelley/ara/aepk_v1_1.tar.gz")

include(ExternalProject)
ExternalProject_Add(photospline
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/photospline
  #URL http://cpc.cs.qub.ac.uk/cpc/cgi-bin/processrequest.pl/?catid=aepk_v1_0
  #URL http://icecube.wisc.edu/~jkelley/ara/aepk_v1_1.tar.gz
  DOWNLOAD_COMMAND curl ${photospline_url} | tar xzv -C ${CMAKE_CURRENT_SOURCE_DIR}
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


