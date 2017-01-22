cmake_minimum_required (VERSION 3.0.2) 

include (GNUInstallDirs)
include (FindPkgConfig) 

set (PROJECT ssh-multithread) 
set (VERSION 1.0.0)
set (CMAKE_INSTALL_PREFIX /usr)
 
project (${PROJECT} C) 
 
file (GLOB_RECURSE SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "src/*.c")
file (GLOB_RECURSE HEADERS RELATIVE ${PROJECT_SOURCE_DIR} "src/*.h")

pkg_search_module (LIBSSH REQUIRED libssh)
pkg_search_module (PTHREAD REQUIRED pthread-stubs)

set (LIBRARIES
	${LIBSSH_LIBRARIES}
	${PTHREAD_LIBRARIES}
	pthread)

set (INCLUDES
	${LIBSSH_INCLUDE_DIRS}
	${PTHREAD_INCLUDE_DIRS})
	
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

include_directories (${INCLUDES})
add_executable (${PROJECT} ${HEADERS} ${SOURCES})
target_link_libraries (${PROJECT} ${LIBRARIES})

install (TARGETS ${PROJECT} RUNTIME DESTINATION /usr/bin)
