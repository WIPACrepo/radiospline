cmake_minimum_required(VERSION 2.8.12)

# Superbuild CMake file for radiospline and photospline library
# J. Kelley
# 
# TO DO
# - figure out how to pass environment to photospline cmake
# - switch back to CPC when source is updated
# - move tables into git
#

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_C_STANDARD 99)

include(ExternalProject)
ExternalProject_Add(photospline
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/photospline
  # FIX ME this is a fork
  GIT_REPOSITORY https://github.com/jkelley/photospline.git
  GIT_TAG master
  # GIT_TAG jk1.0
  UPDATE_COMMAND ""
  INSTALL_DIR ${CMAKE_BINARY_DIR}/install
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)

# Grab the tables from the web
ExternalProject_Add(radiospline_tables
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources/tables/exponential
    URL http://icecube.wisc.edu/~jkelley/ara/icetables_exponential_180125.tar.gz
    URL_MD5 93513504f176f210f8b4a26629b2d8a6
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


