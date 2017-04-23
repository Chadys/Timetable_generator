cmake_minimum_required(VERSION 3.5)
project(Timetable_Generator)

set(CMAKE_CXX_STANDARD 14)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -Wextra -Wall" )
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(ExcelFormat)


set(SOURCE_FILES main.cpp Students.cpp Students.h Course.cpp Course.h Teacher.cpp Teacher.h Time.cpp Time.h Classroom.cpp Classroom.h DataProvider.cpp DataProvider.h Graph.h Graph.cpp Timetable.cpp Timetable.h NRPA.cpp NRPA.h VertexProperty.cpp VertexProperty.h)
add_executable(Timetable_Generator ${SOURCE_FILES})
target_link_libraries(Timetable_Generator ExcelFormat)
