cmake_minimum_required(VERSION 3.0.0)
project(gfxengine VERSION 0.1.0)

include(CTest)

enable_testing()
#SET(CMAKE_CXX_FLAGS "--coverage")

#file(GLOB SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*cpp" "src/*hpp")

#message(STATUS "sources here: ${SOURCES}")

#add_executable(gfxengine ${SOURCES})

#set(CMAKE_CXX_STANDARD 17)

#target_link_libraries(gfxengine PRIVATE glfw vulkan)

include_directories(".")

add_subdirectory(src)

add_subdirectory(test)

add_test(NAME TestDevices COMMAND TestDevices)
add_test(NAME TestGlfwInit COMMAND TestGlfwInit)
add_test(NAME TestWindow COMMAND TestWindow)
add_test(NAME TestPipeline COMMAND TestPipeline)
add_test(NAME TestAutoDevice COMMAND TestAutoDevice)
add_test(NAME TestSurface COMMAND TestSurface)
add_test(NAME TestSwapChain COMMAND TestSwapChain)
add_test(NAME TestDebugMessenger COMMAND TestDebugMessenger)
add_test(NAME TestFrameBuffer COMMAND TestFrameBuffer)
add_test(NAME TestCommand COMMAND TestCommand)
add_test(NAME TestDraw COMMAND TestDraw)


#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
