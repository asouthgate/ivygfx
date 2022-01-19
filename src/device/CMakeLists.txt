#file(GLOB SOURCES RELATIVE "./" "*cpp" "*hpp")

message(STATUS "CMAKE_SOURCE_DIR" ${CMAKE_SOURCE_DIR})

file(GLOB SOURCES RELATIVE ${CMAKE_SOURCE_DIR}"/src/device" 
                                                    "autodevice.hpp"
                                                    "autodevice.cpp"
                                                    "logical_device.hpp"
                                                    "logical_device.cpp"
                                                    "command.hpp"
                                                    "command.cpp"
                                                    "debug_messenger.hpp"
                                                    "debug_messenger.cpp"
                                                    "instance.cpp"
                                                    "instance.hpp"
                                                    "framebuffer.hpp"
                                                    "framebuffer.cpp"
                                                    "window.cpp"
                                                    "window.hpp"
                                                    "renderpass.hpp"
                                                    "renderpass.cpp"
                                                    "swapchain.hpp"
                                                    "swapchain.cpp"
                                                    "pipeline.hpp"
                                                    "pipeline.cpp"
                                                    "physical_device.cpp"
                                                    "physical_device.hpp"
                                                    "surface.hpp"
                                                    "surface.cpp"
                                                    "queue.cpp"
                                                    "queue.hpp"
                                                    "autodevice.hpp")

message(STATUS "sources here: ${SOURCES}")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DBOOST_LOG_DYN_LINK)

add_library(ivydevice SHARED ${SOURCES})
target_link_libraries(ivydevice)
