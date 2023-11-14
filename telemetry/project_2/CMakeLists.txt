cmake_minimum_required(VERSION 3.21)
project(Project2 VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -O0 -g -w")

# Set the C++ compiler (g++.exe) path 
#set(CMAKE_CXX_COMPILER "C:/MinGW/bin/g++.exe")

# Set the C compiler (gcc.exe) path
#set(CMAKE_C_COMPILER "C:/MinGW/bin/gcc.exe")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# List of source files
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/fake_receiver.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/writer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/statistics.cpp
)

#Compile every source file as C++
set_source_files_properties(
    ${SOURCE_FILES}
    PROPERTIES LANGUAGE CXX
)


# Create a library from the source files
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Add any other libraries you need to link against here
target_link_libraries(${PROJECT_NAME}
    #Other libs
)

# Set the output directory for the executable
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Create the executable and link it against your library
add_executable(project_2 ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(project_2 ${PROJECT_NAME} stdc++fs)




# Without creating the library:
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# add_executable(project_2
#     src/main.cpp
#     src/reader.cpp
#     src/fake_receiver.c
# )
