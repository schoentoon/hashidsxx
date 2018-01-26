cmake_minimum_required(VERSION 3.8)

project(hashids VERSION 1.0.0 LANGUAGES CXX)

include(external-projects.cmake)
link_directories(${EXTERNAL_PROJECTS_LIBS_DIR})

add_library(hashids STATIC ${CMAKE_SOURCE_DIR}/hashids.cpp)

add_executable(hashidsxx ${CMAKE_SOURCE_DIR}/hashidsxx.cpp)
target_link_libraries(hashidsxx PRIVATE hashids)

add_executable(hashids_test ${CMAKE_SOURCE_DIR}/test.cpp)
add_dependencies(hashids_test externalGoogleTest)
target_include_directories(hashids_test PRIVATE $<BUILD_INTERFACE:${EXTERNAL_PROJECTS_INCLUDE_DIR}>)
find_package(Threads REQUIRED)
target_link_libraries(hashids_test PRIVATE hashids gtest Threads::Threads)
enable_testing()
add_test(hashids_test hashids_test)

set_target_properties(hashidsxx hashids hashids_test PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    )

include(install.cmake)