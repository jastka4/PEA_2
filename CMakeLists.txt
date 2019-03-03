cmake_minimum_required(VERSION 3.12)
project(Project1)

set(CMAKE_CXX_STANDARD 17)

add_executable(Project1
        src/main.cpp
        include/Graph.cpp
        include/Graph.h
        include/GraphMatrix.cpp
        include/GraphMatrix.h
        include/Timer.cpp
        include/Timer.h
        src/TS.cpp
        src/TS.h
        src/TSP.cpp
        src/TSP.h
        src/SA.cpp
        src/SA.h
        )