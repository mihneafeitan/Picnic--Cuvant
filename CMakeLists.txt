cmake_minimum_required(VERSION 3.16)
project(PicnicCuvant VERSION 1.0 LANGUAGES CXX)

# Standard modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(include)

# ---------------------------------------------------------------
# External dependencies (SFML)
# ---------------------------------------------------------------
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
)
FetchContent_MakeAvailable(SFML)

find_package(Threads REQUIRED)
if(APPLE)
elseif(UNIX)
    find_package(X11)
endif()

# ---------------------------------------------------------------
# Source files
# ---------------------------------------------------------------
set(SOURCES
    main.cpp
    src/Game.cpp
    src/Level.cpp
    src/WordList.cpp
)

# ---------------------------------------------------------------
# Executable
# ---------------------------------------------------------------
add_executable(PicnicCuvant ${SOURCES})

# Set output name
set_target_properties(PicnicCuvant PROPERTIES OUTPUT_NAME "picnic")

# ---------------------------------------------------------------
# Linking
# ---------------------------------------------------------------
target_link_libraries(PicnicCuvant PRIVATE SFML::Graphics SFML::Window SFML::System Threads::Threads)

if(APPLE)
elseif(UNIX)
    target_link_libraries(PicnicCuvant PRIVATE X11)
endif()

# ---------------------------------------------------------------
# Install and extra files
# ---------------------------------------------------------------
set(DESTINATION_DIR bin)
install(TARGETS PicnicCuvant DESTINATION ${DESTINATION_DIR})

# Copy extra files (like tastatura.txt)
if(EXISTS "${CMAKE_SOURCE_DIR}/tastatura.txt")
    file(COPY "${CMAKE_SOURCE_DIR}/tastatura.txt" DESTINATION "${CMAKE_BINARY_DIR}")
endif()

message(STATUS "CMake configuration complete for PicnicCuvant")
