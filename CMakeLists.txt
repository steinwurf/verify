cmake_minimum_required(VERSION 3.15)

project(
    verify
    VERSION 6.3.0 # Managed by waf
    DESCRIPTION "a meta assertion library focused on high compatibility"
    HOMEPAGE_URL "https://github.com/steinwurf/verify"
    LANGUAGES CXX)


find_package(Python COMPONENTS Interpreter)

# Use waf to resolve dependencies
if(NOT DEFINED STEINWURF_RESOLVE)
    message(STATUS "Resolving dependencies...")
    execute_process(
        COMMAND ${Python_EXECUTABLE} waf resolve ${STEINWURF_RESOLVE_OPTIONS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE STATUS)

    if(STATUS AND NOT STATUS EQUAL 0)
        message(FATAL_ERROR "Failed: ${STATUS}")
    endif()

    set(STEINWURF_RESOLVE "${CMAKE_CURRENT_SOURCE_DIR}/resolve_symlinks")
endif()

# Include common CMake settings
include("${STEINWURF_RESOLVE}/toolchains/common_settings.cmake")

# fmt dependency: types with an fmt::formatter are printable in verify output
if(NOT TARGET steinwurf::fmt)
    add_subdirectory("${STEINWURF_RESOLVE}/fmt" fmt EXCLUDE_FROM_ALL)
endif()

file(GLOB_RECURSE verify_sources ./src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${verify_sources})
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_libraries(${PROJECT_NAME} PUBLIC steinwurf::fmt)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_EXTENSIONS OFF)
# The public headers use C++17 (std::string_view, if constexpr, ...), so
# everything that includes them must be compiled as C++17 too
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
# dladdr() is in libdl on glibc before 2.34; used for the stack trace
if(CMAKE_DL_LIBS)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif()

add_library(steinwurf::verify ALIAS ${PROJECT_NAME})

install(
    DIRECTORY ./src
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    FILES_MATCHING
    PATTERN *.hpp)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  file(GLOB_RECURSE verify_example_sources example/*.cpp)
  add_executable(verify_example ${verify_example_sources})
  target_link_libraries(verify_example PUBLIC steinwurf::verify)
endif()

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    enable_testing()

    # Google Test dependency
    add_subdirectory("${STEINWURF_RESOLVE}/gtest" gtest_main EXCLUDE_FROM_ALL)

    # Define library
    file(GLOB_RECURSE verify_test_sources test/*.cpp)

    # Build test executable
    add_executable(verify_tests ${verify_test_sources})
    target_link_libraries(verify_tests PUBLIC steinwurf::verify)
    target_link_libraries(verify_tests PUBLIC steinwurf::gtest)

    # Make sure we compile with C++17 and do not use compiler specific extensions
    set_property(TARGET verify_tests PROPERTY CXX_STANDARD 17)
    set_property(TARGET verify_tests PROPERTY CXX_EXTENSIONS OFF)

    if(MSVC)
        target_compile_options(verify_tests PRIVATE /W4)

        # Make Release builds explicitly keep NDEBUG unset.
        string(REGEX REPLACE "/DNDEBUG" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
        string(REGEX REPLACE "/DNDEBUG" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    else()
        target_compile_options(verify_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
        # Function names in the stack trace of a failed VERIFY
        set_property(TARGET verify_tests PROPERTY ENABLE_EXPORTS ON)

        # Make Release builds explicitly keep NDEBUG unset.
        string(REGEX REPLACE "-DNDEBUG" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
        string(REGEX REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    endif()

    add_test(verify_tests verify_tests)

    # Benchmark: the same scenarios compiled once per backend and linked into
    # one binary, so all backends are compared on the same machine and run.
    add_executable(verify_benchmark
        benchmark/main.cpp
        benchmark/bench_ndebug.cpp
        benchmark/bench_cassert.cpp
        benchmark/bench_native.cpp)
    target_link_libraries(verify_benchmark PRIVATE steinwurf::verify)
    set_property(TARGET verify_benchmark PROPERTY CXX_STANDARD 17)
    set_property(TARGET verify_benchmark PROPERTY CXX_EXTENSIONS OFF)
    if(NOT MSVC)
        target_compile_options(verify_benchmark PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
endif()
