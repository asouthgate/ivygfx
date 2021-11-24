#file(GLOB SOURCES RELATIVE "./" "*cpp" "*hpp")

message(STATUS "CMAKE_SOURCE_DIR" ${CMAKE_SOURCE_DIR})

file(GLOB SOURCES RELATIVE ${CMAKE_SOURCE_DIR}"/src" "*cpp" "*hpp")

message(STATUS "sources here: ${SOURCES}")

add_executable(Ivy ${SOURCES})

set(CMAKE_CXX_STANDARD 17)

target_link_libraries(Ivy PRIVATE glfw vulkan)

