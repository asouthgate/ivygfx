#file(GLOB SOURCES RELATIVE "./" "*cpp" "*hpp")

include_directories("./device")

message(STATUS "CMAKE_SOURCE_DIR" ${CMAKE_SOURCE_DIR})

file(GLOB SOURCES RELATIVE ${CMAKE_SOURCE_DIR}"/src" 
                                                    "device/logical_device.hpp"
                                                    "device/logical_device.cpp"
                                                    "instance.hpp"
                                                    "validation.cpp" 
                                                    "validation.hpp"
                                                    "ive_window.cpp"
                                                    "ive_window.hpp"
                                                    "device/physical_device.cpp"
                                                    "device/physical_device.hpp"
                                                    "device/queue.cpp"
                                                    "device/queue.hpp")

message(STATUS "sources here: ${SOURCES}")

set(CMAKE_BUILD_TYPE Debug)

add_definitions(-DBOOST_LOG_DYN_LINK)

add_library(ivylib SHARED ${SOURCES})

#add_executable(Ivy ${SOURCES})

#set(CMAKE_CXX_STANDARD 17)

#target_link_libraries(ivylib PRIVATE glfw vulkan)

