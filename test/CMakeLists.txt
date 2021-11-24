find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

include_directories (${IVY_SOURCE_DIR}/src
                     ${Boost_INCLUDE_DIRS}
                     )

add_definitions (-DBOOST_TEST_DYN_LINK)

add_executable(Test1 test.cpp)

# also link to any libraries produced in Ivy project

target_link_libraries(Test1
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )
