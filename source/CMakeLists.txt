#######################################################
# cmake file for building SDHCALEventDisplay package
# @author Eté Rémi 
#######################################################

SET( ${PROJECT_NAME}_INCLUDE_DIR ${${PROJECT_NAME}_SOURCE_DIR}/source/include )
INCLUDE_DIRECTORIES( ${${PROJECT_NAME}_INCLUDE_DIR} )

REMOVE_DEFINITIONS("O2")
ADD_DEFINITIONS(-pthread)
ADD_DEFINITIONS(-O0)

FILE(
  GLOB_RECURSE
  ${PROJECT_NAME}_SRC_FILES
  src/*
)

FILE(
  GLOB_RECURSE
  ${PROJECT_NAME}_INCLUDE_FILES
  include/*
)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/src )

ADD_SHARED_LIBRARY( 
  ${PROJECT_NAME} 
  ${${PROJECT_NAME}_SRC_FILES} 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}Dict.cc
)

INSTALL(
  TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION lib 
)

########## ROOT CINT DICT ###########


set ( 
  ${PROJECT_NAME}DictHeaders 
  ${CMAKE_CURRENT_SOURCE_DIR}/include/EventNavigator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/EventManager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/LcioCollectionFrame.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/DetectorStatisticsFrame.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/DetectorStatisticsWindow.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/DetectorFrame.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/CaloHitFrame.h
)

SET( DICT_INCLUDE_DIRS "" )
GET_DIRECTORY_PROPERTY(IncludeDirs INCLUDE_DIRECTORIES)

FOREACH( dir ${IncludeDirs} )
   SET( DICT_INCLUDE_DIRS ${DICT_INCLUDE_DIRS}\t-I${dir} )
ENDFOREACH(dir)


MESSAGE( "ROOT CINT command : ${ROOT_CINT_EXECUTABLE}" )


######################################################

SET( LIB_HEADERS_PKG ${${PROJECT_NAME}DictHeaders} )

SET( LINK_DEF_FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/LinkDef.h )

SET( ROOT_DICT_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )

INCLUDE( MacroRootDict )

add_custom_command( OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}Dict.cc
                   COMMAND mkdir -p ${ROOT_DICT_OUTPUT_DIR}
                   COMMAND ${ROOT_CINT_WRAPPER} -v4 -f ${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}Dict.cc -c ${DICT_INCLUDE_DIRS} -p ${LIB_HEADERS_PKG} ${LINK_DEF_FILE}
                   COMMENT "Generating ROOT dictionary for ${PROJECT_NAME}."
                   DEPENDS ${LIB_HEADERS_PKG}
)

######################################################



ADD_EXECUTABLE( sdhcal_event_display main/sdhcal_event_display.cc  )
TARGET_LINK_LIBRARIES( sdhcal_event_display ${PROJECT_NAME} ${ROOT_GUI_LIBRARY} ${ROOT_EVE_LIBRARY} ${ROOT_GEOM_LIBRARY} )

INSTALL(
  TARGETS sdhcal_event_display
  RUNTIME DESTINATION bin 
)

