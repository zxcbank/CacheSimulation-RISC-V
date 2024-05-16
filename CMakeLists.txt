cmake_minimum_required(VERSION 3.28)
project(ARCH3)

set(CMAKE_CXX_STANDARD 20)

add_executable(ARCH3
        main.cpp
        include/ArgParser.cpp
        include/processor.cpp
        include/regesters.hpp
        include/ram.hpp
        include/RiskVcommand.cpp
        include/RISK32I.hpp
        include/RISK32M.hpp
        include/regesters.cpp
        include/RISK32I.cpp
        include/RISK32M.cpp
        include/Lru.hpp
        include/Lru.cpp
        include/Plru.hpp
        include/Plru.cpp
)
