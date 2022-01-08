#file(GLOB SOURCES RELATIVE "./" "*cpp" "*hpp")

message(STATUS "CMAKE_SOURCE_DIR" ${CMAKE_SOURCE_DIR})

file(GLOB SOURCES RELATIVE ${CMAKE_SOURCE_DIR}"/src/device" 
                                                    "logical_device.hpp"
                                                    "logical_device.cpp"
                                                    "debug_messenger.hpp"
                                                    "debug_messenger.cpp"
                                                    "instance.cpp"
                                                    "instance.hpp"
                                                    "ive_window.cpp"
                                                    "ive_window.hpp"
                                                    "swapchain.hpp"
                                                    "swapchain.cpp"
                                                    "physical_device.cpp"
                                                    "physical_device.hpp"
                                                    "queue.cpp"
                                                    "queue.hpp"
                                                    "autodevice.hpp")

message(STATUS "sources here: ${SOURCES}")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DBOOST_LOG_DYN_LINK)

add_library(ivydevicelib SHARED ${SOURCES})

#add_executable(Ivy ${SOURCES})


#target_link_libraries(ivylib PRIVATE glfw vulkan)

