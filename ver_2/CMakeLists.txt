cmake_minimum_required(VERSION 3.10)
project(TeknicSystem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system thread)
if(Boost_FOUND)
    message(STATUS "Found Boost: ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
    list(APPEND EXTRA_LIBS ${Boost_LIBRARIES})
endif()

set(SFOUNDATION_DIR ${CMAKE_SOURCE_DIR}/Dependencies/common/sFoundation/sFoundation)
set(ADS_DIR ${CMAKE_SOURCE_DIR}/Dependencies/common/ADS)

set(BUILD_SHARED_LIBS OFF)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)

add_subdirectory(${SFOUNDATION_DIR} sFoundation_build)
add_subdirectory(${ADS_DIR} ADSbuild)

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
    src/TwincatADSNode.cpp
    src/utils.cpp
)

add_executable(main ${SOURCES})

set_target_properties(main PROPERTIES
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib;${SFOUNDATION_DIR}"
    BUILD_WITH_INSTALL_RPATH TRUE
)

target_link_libraries(main PRIVATE
    sFoundation
    ads
)
if(UNIX)
    target_link_libraries(main PRIVATE pthread rt dl)
endif()

install(TARGETS main DESTINATION bin)
install(FILES 
    ${SFOUNDATION_DIR}/MNuserDriver20.xml
    DESTINATION lib
)
