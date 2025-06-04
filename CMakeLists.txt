cmake_minimum_required(VERSION 3.15)

project(
    verify
    VERSION 6.0.0 # Managed by waf
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

if(NOT DEFINED STEINWURF_LIBASSERT)
    message(STATUS "STEINWURF_LIBASSERT not defined, using ON")

    set(STEINWURF_LIBASSERT ON)
endif()

add_library(${PROJECT_NAME} STATIC src/verify.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_EXTENSIONS OFF)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Enable pedantic errors
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
# GCC is throwing ISO errors from abseil that we can't fix
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Clang gets really mad at libassert for using zero arguments internally to
    # `...` VA_ARGS.
    target_compile_options(${PROJECT_NAME} PUBLIC 
        -Wno-c++20-extensions
        -Wno-gnu-zero-variadic-macro-arguments)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    # Clang gets really mad at libassert for using zero arguments internally to
    # `...` VA_ARGS.
    target_compile_options(${PROJECT_NAME} PUBLIC 
        -Wno-c++20-extensions
        -Wno-gnu-zero-variadic-macro-arguments)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC )
endif()

add_library(steinwurf::verify ALIAS ${PROJECT_NAME})

install(
    DIRECTORY ./include
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    FILES_MATCHING
    PATTERN *.hpp)

if(STEINWURF_LIBASSERT)

    if(NOT TARGET ZLIB::ZLIB)
        find_package(ZLIB)

        if(NOT ZLIB_FOUND)
            set(ZLIB_BUILD_EXAMPLES NO)
            add_subdirectory("${STEINWURF_RESOLVE}/zlib-source" zlib EXCLUDE_FROM_ALL)
        endif()
    endif()

    if(NOT TARGET libassert::assert)
        add_subdirectory("${STEINWURF_RESOLVE}/libassert-source" libassert EXCLUDE_FROM_ALL)
    endif()

    target_link_libraries(${PROJECT_NAME} PUBLIC libassert::assert)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LIBASSERT_PREFIX_ASSERTIONS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC STEINWURF_LIBASSERT)
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

        # Make Release builds explicitly keep NDEBUG unset.
        string(REGEX REPLACE "-DNDEBUG" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
        string(REGEX REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    endif()

    add_test(verify_tests verify_tests)
endif()
