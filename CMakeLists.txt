cmake_minimum_required(VERSION 3.0.0)
project(gfxengine VERSION 0.1.0)

include(CTest)

enable_testing()

#file(GLOB SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*cpp" "src/*hpp")

#message(STATUS "sources here: ${SOURCES}")

#add_executable(gfxengine ${SOURCES})

#set(CMAKE_CXX_STANDARD 17)

#target_link_libraries(gfxengine PRIVATE glfw vulkan)

include_directories(".")

add_subdirectory(src)

add_subdirectory(test)

add_test(NAME FirstTest COMMAND Test1)

add_test(NAME TestDevice COMMAND TestDevice)

add_test(NAME TestInit COMMAND TestInit)

add_test(NAME TestPipeline COMMAND TestPipeline)


#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
