cmake_minimum_required(VERSION 3.6)

set(APP_NAME remesh)

set(APP_SRC main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Packages
find_package(CGAL REQUIRED COMPONENTS Core)
include( ${CGAL_USE_FILE} )

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS 
  program_options 
  system
  filesystem)


include_directories(
  ${CGAL_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ${GMP_INCLUDE_DIRS})

link_directories(${Boost_LIBRARY_DIR})


add_executable(${APP_NAME} ${APP_SRC})

target_link_libraries(
  ${APP_NAME}  
  ${Boost_LIBRARIES}
  ${GMP_LIBRARIES}
  ${CGAL_LIBRARIES})

