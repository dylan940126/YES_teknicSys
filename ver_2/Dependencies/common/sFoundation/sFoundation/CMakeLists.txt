# This CMakeLists.txt file was converted from a Makefile by AI.


cmake_minimum_required(VERSION 3.10)
project(sFoundation LANGUAGES CXX)

file(GLOB SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src-linux/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../LibINI/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../LibLinuxOS/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../LibXML/src/*.cpp"
)

add_library(sFoundation ${SRC_FILES})

target_include_directories(sFoundation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/inc-pub
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/inc-private
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/inc-private/linux
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/inc-private/sFound
    ${CMAKE_CURRENT_SOURCE_DIR}/../LibINI/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../LibLinuxOS/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../LibXML/inc
)

target_compile_features(sFoundation PUBLIC cxx_std_11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(UNIX)
    target_link_libraries(sFoundation PUBLIC rt dl)
endif()

# set_target_properties(sFoundation PROPERTIES
#     OUTPUT_NAME "libsFoundation20"
#     VERSION 1.0
#     SOVERSION 1
# )

install(TARGETS sFoundation
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)