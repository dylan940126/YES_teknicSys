cmake_minimum_required(VERSION 3.10)
project(TeknicSystem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(Boost REQUIRED COMPONENTS system thread)
if(Boost_FOUND)
    message(STATUS "Found Boost: ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
    list(APPEND EXTRA_LIBS ${Boost_LIBRARIES})
endif()

set(SFOUNDATION_DIR ${CMAKE_SOURCE_DIR}/Dependencies/linux/sFoundation/sFoundation)
set(ADS_DIR ${CMAKE_SOURCE_DIR}/Dependencies/common/ADS)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${SFOUNDATION_DIR}/inc/inc-pub
)

set(SFOUNDATION_LIB ${SFOUNDATION_DIR}/libsFoundation20.so)

if(NOT EXISTS ${SFOUNDATION_LIB})
    message(STATUS "sFoundation library does not exist, building automatically...")
    execute_process(
        COMMAND make
        WORKING_DIRECTORY ${SFOUNDATION_DIR}
    )
endif()

set(SOURCES
    src/BrakeController.cpp
    src/CableController.cpp
    src/COMPortNode.cpp
    src/DeltaClient.cpp
    src/GripperController.cpp
    src/Logger.cpp
    src/Main.cpp
    src/RailController.cpp
    src/Robot.cpp
    src/TeknicNode.cpp
    src/TrajectoryGenerator.cpp
    src/utils.cpp
)

set(SOURCES ${SOURCES} src/TwincatADSNode.cpp)
add_subdirectory(${ADS_DIR} ADSbuild)
list(APPEND EXTRA_LIBS ads)
include_directories(${ADS_DIR}/AdsLib/include)

add_executable(main ${SOURCES})

set_target_properties(main PROPERTIES
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib;${SFOUNDATION_DIR}"
    BUILD_WITH_INSTALL_RPATH TRUE
)

target_link_libraries(main
    ${SFOUNDATION_LIB}
    ${EXTRA_LIBS}
    pthread
    rt
    dl
)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${SFOUNDATION_DIR}/libsFoundation20.so
        ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
        ${SFOUNDATION_DIR}/MNuserDriver20.xml
        ${CMAKE_BINARY_DIR}/
)

# 安裝目標
install(TARGETS main DESTINATION bin)
install(FILES 
    ${SFOUNDATION_DIR}/libsFoundation20.so
    ${SFOUNDATION_DIR}/MNuserDriver20.xml
    DESTINATION lib
)
