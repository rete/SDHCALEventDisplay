#################################################
# cmake file for building SDHCALEventDisplay binary
# @author Eté Rémi
# Copyright IPNL/CNRS 2014
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 FATAL_ERROR )
#################################################

# project name
PROJECT( SDHCALEventDisplay )

# ---------------------------------------------------------------------------------------------------
#
# Needed package for SDHCALEventDisplay
#
#   - ilcutil
#   - streamlog
#   - LCIO
#   - ROOT
#   - GEAR
# ---------------------------------------------------------------------------------------------------


SET( ${PROJECT_NAME}_VERSION_MAJOR 1 )
SET( ${PROJECT_NAME}_VERSION_MINOR 0 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )

# ----------------
# default settings
# ----------------

LIST( APPEND CMAKE_MODULE_PATH $ILCSOFT )
LIST( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

# ------------------------------------------------
# load default settings from ILCSOFT_CMAKE_MODULES
# ------------------------------------------------

FIND_PACKAGE( ILCUTIL COMPONENTS ILCSOFT_CMAKE_MODULES streamlog REQUIRED )
INCLUDE( ilcsoft_default_settings )

# -------------------
# load cmake packages
# -------------------

FIND_PACKAGE( ROOT REQUIRED 
  COMPONENTS 
  Core RIO Net 
  Hist Graf Graf3d 
  Gpad Tree Rint 
  Matrix Physics MathCore 
  Thread Gui Geom 
  TreePlayer RGL GeomPainter 
  Eve 
)
FIND_PACKAGE( LCIO REQUIRED )
FIND_PACKAGE( GEAR REQUIRED )

FOREACH( pkg ROOT LCIO GEAR streamlog )
  IF( ${pkg}_FOUND )
    INCLUDE_DIRECTORIES( ${${pkg}_INCLUDE_DIRS} )
    LINK_LIBRARIES( ${${pkg}_LIBRARIES} )
    ADD_DEFINITIONS ( ${${pkg}_DEFINITIONS} )
  ELSE()
    MESSAGE( FATAL "Package ${pkg} not found. Please source the ILCSOFT environment before running cmake" )
  ENDIF()
ENDFOREACH()


CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Version.h.cmake.in"
    "${PROJECT_SOURCE_DIR}/source/include/${PROJECT_NAME}Version.h" @ONLY )

# ------------------------------------------
# include the top CMakeLists.txt for sources
# ------------------------------------------

INCLUDE_DIRECTORIES( source )

ADD_SUBDIRECTORY( ./source )

DISPLAY_STD_VARIABLES()
