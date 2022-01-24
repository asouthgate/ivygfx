find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

include_directories (${IVY_SOURCE_DIR}/src
                     ${Boost_INCLUDE_DIRS}
                     )

add_definitions (-DBOOST_TEST_DYN_LINK)
add_definitions(-DBOOST_LOG_DYN_LINK)

set(CMAKE_CXX_STANDARD 17)

add_executable(TestGlfwInit test_glfw_init.cpp)
add_executable(TestDevices test_devices.cpp)
add_executable(TestWindow test_window.cpp)
add_executable(TestPipeline test_pipeline.cpp)
add_executable(TestAutoDevice test_autodevice.cpp)
add_executable(TestSurface test_surface.cpp)
add_executable(TestDebugMessenger test_debug_messenger.cpp)
add_executable(TestSwapChain test_swapchain.cpp)
add_executable(TestFrameBuffer test_framebuffer.cpp)
add_executable(TestCommand test_command.cpp)
add_executable(TestDraw test_draw.cpp)

# also link to any libraries produced in Ivy project

target_link_libraries(TestWindow
                        ${Boost_FILESYSTEM_LIBRARY}
                        ${Boost_SYSTEM_LIBRARY}
                        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ivydevice vulkan glfw pthread boost_log
)

target_link_libraries(TestGlfwInit
                        ${Boost_FILESYSTEM_LIBRARY}
                        ${Boost_SYSTEM_LIBRARY}
                        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ivydevice vulkan glfw pthread boost_log
)


target_link_libraries(TestDevices
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_LOG_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ivydevice vulkan glfw pthread boost_log
                       )

target_link_libraries(TestPipeline
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_LOG_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ivydevice  vulkan glfw pthread boost_log
                       )

target_link_libraries(TestAutoDevice
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_LOG_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ivydevice  vulkan glfw pthread boost_log
                       )

target_link_libraries(TestSurface
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_LOG_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ivydevice  vulkan glfw pthread boost_log
                       )

target_link_libraries(TestDebugMessenger
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_LOG_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ivydevice  vulkan glfw pthread boost_log
                       )

target_link_libraries(TestSwapChain
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_LOG_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ivydevice  vulkan glfw pthread boost_log
                       )

target_link_libraries(TestFrameBuffer
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_LOG_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ivydevice  vulkan glfw pthread boost_log
                       )

target_link_libraries(TestCommand
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_LOG_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ivydevice vulkan glfw pthread boost_log
                       )

target_link_libraries(TestDraw
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_LOG_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ivydevice vulkan glfw pthread boost_log
                       )