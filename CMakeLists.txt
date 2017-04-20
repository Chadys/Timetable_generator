cmake_minimum_required(VERSION 3.5)
project(Timetable_Generator)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp Students.cpp Students.h Course.cpp Course.h Teacher.cpp Teacher.h Time.cpp Time.h Classroom.cpp Classroom.h DataProvider.cpp DataProvider.h Graph.h Timetable.cpp Timetable.h NRPA.cpp NRPA.h GraphProperty.cpp GraphProperty.h Graph.cpp.cpp)
add_executable(Timetable_Generator ${SOURCE_FILES})