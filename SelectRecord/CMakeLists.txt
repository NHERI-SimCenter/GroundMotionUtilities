cmake_minimum_required(VERSION 3.0.0)

set (CMAKE_CXX_STANDARD 11)

project (SelectGM VERSION 0.0.1)

include_directories("include")
include_directories("../include")

file(GLOB SOURCES "src/*.cpp" "src/*.c")
                    
add_executable(SelectRecord ${SOURCES})

add_custom_command(TARGET SelectRecord POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/examples $<TARGET_FILE_DIR:SelectRecord>
                    COMMENT "Copying examples...")

install(TARGETS SelectRecord
        DESTINATION ${CMAKE_SOURCE_DIR}/build/bin)