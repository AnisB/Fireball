cmake_minimum_required(VERSION 2.6)

#defining the compiler
#SET(CMAKE_CXX_COMPILER "g++-4.9")
#SET(CMAKE_C_COMPILER "gcc-4.9")

#Projet definition
project(Snowman)


#Definiton des répertoires
set(SOURCES "${PROJECT_SOURCE_DIR}/src/")
set(TEST "${PROJECT_SOURCE_DIR}/test/")
set(PROGRAM "${PROJECT_SOURCE_DIR}/program/")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/;${CMAKE_MODULE_PATH}")


#Debug flags
# add_definitions( -DDEBUG )
# SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

#Recursive source parsing
file(
  GLOB_RECURSE 
  Source_File 
  ${SOURCES}/*.h
  ${SOURCES}/*.ih
  ${SOURCES}/*.cpp
)


#Compiler flags
ADD_DEFINITIONS(-std=c++11)

#External libraries links
IF(UNIX AND NOT APPLE)
  set(EXTERNAL_LIBS "${PROJECT_SOURCE_DIR}/external/linux")
  set(EXTERNAL_INCLUDES "${PROJECT_SOURCE_DIR}/include/linux")
ENDIF()

IF(APPLE)
  set(EXTERNAL_LIBS "${PROJECT_SOURCE_DIR}/external/mac")
  set(EXTERNAL_INCLUDES "${PROJECT_SOURCE_DIR}/include/mac")
  LINK_DIRECTORIES("/usr/local/lib")
ENDIF()

IF(UNIX AND NOT APPLE)
    #Preprocessing macros
    add_definitions( -D__posix__ )
    add_definitions( -D LINUX )

    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    FIND_PACKAGE (X11 REQUIRED)
    FIND_PACKAGE( Boost 1.40 COMPONENTS program_options system REQUIRED  )
    
    #Linking libraries
    set(GRAPHIC_LIBS "${EXTERNAL_LIBS}/libglfw3.a" "/usr/lib64/librt.so" ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${Boost_LIBRARIES} 
    ${X11_Xxf86vm_LIB} ${X11_Xcursor_LIB} ${X11_Xinerama_LIB} ${X11_Xrandr_LIB} ${X11_Xi_LIB} pthread ${X11_X11_LIB})
    SET(IMAGE_LIBS jpeg png)
    SET(CORELIBS ${GRAPHIC_LIBS} ${IMAGE_LIBS} ${extra_libs})

    #Setting include directories
    include_directories(${SOURCES} ${OPENGL_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${EXTERNAL_INCLUDES} )
endif()
IF(APPLE)
  #Preprocessing macros
  add_definitions( -D__posix__ )
  add_definitions( -DMACOSX )


  FIND_LIBRARY(OPENCL_LIBRARY OpenCL REQUIRED)
  add_definitions(-DOPENCL)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_PACKAGE( Boost 1.40 COMPONENTS program_options system REQUIRED )
  FIND_LIBRARY(IOKIT_LIBRARY IOKit)
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
  find_package(GLEW REQUIRED)
  MARK_AS_ADVANCED (COCOA_LIBRARY GLFW_LIBRARY OpenGL_LIBRARY IOKIT_LIBRARY OPENCL_LIBRARY)

  #Linking libraries       
  SET(GRAPHIC_LIBS ${COCOA_LIBRARY} ${GLEW_LIBRARY} ${OpenGL_LIBRARY} ${OPENCL_LIBRARY} ${IOKIT_LIBRARY} ${Boost_LIBRARIES} "${EXTERNAL_LIBS}/libglfw3.a")
  SET(IMAGE_LIBS jpeg png)
  SET(CORELIBS ${CORELIBS} ${GRAPHIC_LIBS} ${IMAGE_LIBS} ${extra_libs})

  include_directories(${SOURCES} ${OPENGL_INCLUDE_DIRS}  ${Boost_INCLUDE_DIR} ${EXTERNAL_INCLUDES})
ENDIF (APPLE)

add_library(Snowman ${Source_File})
 
link_directories(${CMAKE_BINARY_DIR})


add_custom_command(
    TARGET Snowman POST_BUILD
    COMMAND ln -sf ${PROJECT_SOURCE_DIR}/data ${PROJECT_BINARY_DIR}
)

#add_subdirectory(${TEST})
add_subdirectory(${PROGRAM})

