cmake_minimum_required(VERSION 3.8)

project(OpenGLSamples)

add_definitions(-DGLEW_STATIC)
add_definitions(-DGLEW_NO_GLU)

find_package(OpenGL)

set(OPEN_SOURCE_DIR "E:/opensource/")
set(GLFW_INCLUDE_DIR "${OPEN_SOURCE_DIR}glfw/include" CACHE PATH "glfw include dir")
set(GLEW_INCLUDE_DIR "${OPEN_SOURCE_DIR}glew/glew_110_include" CACHE PATH "glew include dir")

set(GLFW_LIBRARY_PATH "${OPEN_SOURCE_DIR}glfw/lib/Win32/Debug/" CACHE PATH "glfw lib path")
set(GLEW_LIBRARY_PATH "${OPEN_SOURCE_DIR}glew/glew_110_lib/Win32/Debug/" CACHE PATH "glew lib path")

find_library(GLFW_LIBRARY NAMES glfw3 PATHS ${GLFW_LIBRARY_PATH})
find_library(GLEW_LIBRARY NAMES glew PATHS ${GLEW_LIBRARY_PATH}) 

add_library(Common STATIC
Common/OpenGLSampleBase.cpp
Common/OpenGLSampleBase.h
)

set(Common_INCLUDE_DIR "Common/")

target_include_directories(Common PRIVATE ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

target_link_libraries(Common ${GLEW_LIBRARY} ${GLFW_LIBRARY} ${OPENGL_gl_LIBRARY})

MACRO(SETUP_SAMPLE_START EXE_NAME)
	UNSET(SOURCE_FILES)
	UNSET(HEADER_FILES)
	UNSET(RESOURCE_FILES)
	UNSET(EFFECT_FILES)
ENDMACRO(SETUP_SAMPLE_START)

MACRO(SETUP_SAMPLE_END EXE_NAME)
	SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
	SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})
	SOURCE_GROUP("Resource Files" FILES ${RESOURCE_FILES})
	SOURCE_GROUP("Effect Files" FILES ${EFFECT_FILES})
	
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/${EXE_NAME})
	
	ADD_EXECUTABLE(${EXE_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES} ${EFFECT_FILES})
	
	target_include_directories(${EXE_NAME} PRIVATE ${Common_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
	
	target_link_libraries(${EXE_NAME} Common ${GLEW_LIBRARY} ${GLFW_LIBRARY} ${OPENGL_gl_LIBRARY})

ENDMACRO(SETUP_SAMPLE_END)

SETUP_SAMPLE_START(Instancing)
	SET(SOURCE_FILES
		Instancing/Instancing.cpp
	)
	
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/HelloOpenGLES/grass.bmp ${CMAKE_CURRENT_BINARY_DIR}/grass.bmp COPYONLY)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/HelloOpenGLES/grass.bmp ${CMAKE_CURRENT_BINARY_DIR}/bin/Instancing/Debug/grass.bmp COPYONLY) 
SETUP_SAMPLE_END(Instancing)

SETUP_SAMPLE_START(HelloOpenGLES)
	SET(SOURCE_FILES
		HelloOpenGLES/HelloOpenGLES.cpp
	)
	SET(RESOURCE_FILES
		HelloOpenGLES/grass.bmp)
			
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/HelloOpenGLES/grass.bmp ${CMAKE_CURRENT_BINARY_DIR}/grass.bmp COPYONLY)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/HelloOpenGLES/grass.bmp ${CMAKE_CURRENT_BINARY_DIR}/bin/HelloOpenGLES/Debug/grass.bmp COPYONLY) 
SETUP_SAMPLE_END(HelloOpenGLES)


SETUP_SAMPLE_START(HelloOpenGL)
	SET(SOURCE_FILES
		HelloOpenGL/HelloOpenGL.cpp
	)
SETUP_SAMPLE_END(HelloOpenGL)

SETUP_SAMPLE_START(PipeLine)
	SET(SOURCE_FILES
		PipeLine/PipeLine.cpp
	)
SETUP_SAMPLE_END(PipeLine)
