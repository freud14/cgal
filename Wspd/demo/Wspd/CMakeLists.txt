# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

project( Wspd )


cmake_minimum_required(VERSION 2.8.11)

# CGAL and its components
find_package( CGAL QUIET COMPONENTS Core Qt5 )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()

endif()

# include helper file
include( ${CGAL_USE_FILE} )


# Boost and its components
find_package( Boost REQUIRED )

find_package(Qt5 QUIET COMPONENTS Xml Script OpenGL Svg)


if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()

endif()

# include for local directory
include_directories( BEFORE include )

# include for local package
include_directories( BEFORE ../../include )


# Creating entries for all C++ files with "main" routine
# ##########################################################

include( CGAL_CreateSingleSourceCGALProgram )

create_single_source_cgal_program( "demo_wspd_3.cpp" )


# Creating entries for target: demo_wspd_2
# ############################
if ( CGAL_FOUND AND CGAL_Qt5_FOUND AND Qt5_FOUND )

qt5_wrap_ui( DT_UI_FILES wspd_2/mainwindow.ui )

# use the Qt MOC preprocessor on classes that derives from QObject
qt5_generate_moc( "wspd_2/mainwindow.h" "${CMAKE_CURRENT_BINARY_DIR}/mainwindow.moc" )

# The executable itself.
add_executable  ( demo_wspd_2 wspd_2/demo_wspd_2.cpp wspd_2/mainwindow.cpp mainwindow.moc ${DT_UI_FILES} ${DT_RESOURCE_FILES} )

qt5_use_modules(demo_wspd_2 Xml Script OpenGL Svg)

add_to_cached_list( CGAL_EXECUTABLE_TARGETS main )

# Link with Qt libraries
target_link_libraries( demo_wspd_2 ${QT_LIBRARIES} )
# Link with CGAL
target_link_libraries( demo_wspd_2 ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})

else()

  message(STATUS "NOTICE: This demo requires CGAL and Qt5, and will not be compiled.")

endif()
