cmake_minimum_required (VERSION 3.15)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Link to CPM
include("CPM.cmake")

# Fetch dependencies using CPM
CPMAddPackage("gh:SFML/SFML#3.0.2")
CPMAddPackage("gh:microsoft/GSL@4.2.0")
CPMAddPackage("gh:nlohmann/json@3.12.0")

# Misc dependency config jobs
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build SFML as static libraries" FORCE)

file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/resources")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

project("Iridium")
add_subdirectory("Iridium")