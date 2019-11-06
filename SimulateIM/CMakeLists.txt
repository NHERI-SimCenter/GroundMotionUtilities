cmake_minimum_required(VERSION 3.0.0)

set (CMAKE_CXX_STANDARD 11)

project (SimulateGM VERSION 0.0.1)

include_directories("include")
include_directories("../include")

file(GLOB SOURCES "src/*.cpp" "src/*.c")
                    
add_executable(SimulateIM ${SOURCES})

add_custom_command(TARGET SimulateIM POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/examples $<TARGET_FILE_DIR:SimulateIM>
                    COMMENT "Copying examples...")

install(TARGETS SimulateIM
        DESTINATION ${CMAKE_SOURCE_DIR}/build/bin)