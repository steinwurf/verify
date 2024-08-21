cmake_minimum_required(VERSION 3.12)

project(
    verify
    VERSION 2.0.0 # Managed by waf
    DESCRIPTION "a meta assertion library focused on high compatibility"
    HOMEPAGE_URL "https://github.com/steinwurf/verify"
    LANGUAGES CXX
)

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
    set(STEINWURF_TOP_NAME ${PROJECT_NAME})
endif()

if(NOT DEFINED STEINWURF_VERIFY_USE_LIBASSERT)
    message(STATUS "STEINWURF_VERIFY_USE_LIBASSERT not defined, using autodetection")
    if(WIN32)
        # Currently libassert doesn't seem to build against windows with our wrapping
        # https://github.com/steinwurf/libassert/pull/1
        set(STEINFWURF_VERIFY_USE_LIBASSERT NO)
        message(STATUS "STEINWURF_VERIFY_USE_LIBASSERT autodetected to NO due to WIN32 environment")
    else()
        # MacOS / *UNIX-likes build correctly with libassert. Enable these by default.
        set(STEINWURF_VERIFY_USE_LIBASSERT YES)
        message(STATUS "STEINWURF_VERIFY_USE_LIBASSERT autodetected to YES")
    endif()
endif()

add_library(${PROJECT_NAME} STATIC src/verify.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_EXTENSIONS OFF)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
add_library(steinwurf::verify ALIAS ${PROJECT_NAME})

install(
    DIRECTORY ./include
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    FILES_MATCHING
    PATTERN *.hpp)

if (MSVC)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(STEINWURF_VERIFY_MSVC_MT_TYPE "/MTd")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(STEINWURF_VERIFY_MSVC_MT_TYPE "/MT")
    endif()

    target_compile_options(${PROJECT_NAME} PUBLIC ${STEINWURF_VERIFY_MSVC_MT_TYPE})
    # Use the conforming preprocessor for MSVC
    target_compile_options(${PROJECT_NAME} PUBLIC "/Zc:preprocessor")
endif()

if(STEINWURF_VERIFY_USE_LIBASSERT)
    if (NOT TARGET steinwurf::libassert)
        add_subdirectory("${STEINWURF_RESOLVE}/libassert" libassert EXCLUDE_FROM_ALL)
    endif()
    target_link_libraries(${PROJECT_NAME} PUBLIC steinwurf::libassert)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LIBASSERT_PREFIX_ASSERTIONS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC STEINWURF_VERIFY_USE_LIBASSERT)
endif()

if (${PROJECT_IS_TOP_LEVEL})
    enable_testing()

    if (CMAKE_C_COMPILER_ID MATCHES "MSVC")
       # For Windows: Prevent overriding the parent project's compiler/linker
       # settings
       set(gtest_force_shared_crt
               ON
               CACHE BOOL "" FORCE)
    endif ()

# Google Test dependency
    add_subdirectory("${STEINWURF_RESOLVE}/gtest-source" gtest_main EXCLUDE_FROM_ALL)
# Define library
    file(GLOB_RECURSE verify_test_sources test/**/*.cpp)

# Build test executable
    add_executable(verify_tests ${verify_test_sources})
    target_link_libraries(verify_tests PUBLIC steinwurf::verify)
    target_link_libraries(verify_tests PUBLIC gtest_main)

    if(MSVC)
        target_compile_options(${PROJECT_NAME} PUBLIC ${STEINWURF_VERIFY_MSVC_MT_TYPE})
    endif()


# Make sure we compile with C++17 and do not use compiler specific extensions
    set_property(TARGET verify_tests PROPERTY CXX_STANDARD 17)
    set_property(TARGET verify_tests PROPERTY CXX_EXTENSIONS OFF)

    # CTest needs an extra helping hand to find MSVC complied exes.
    if(MSVC)
        add_test(NAME verify_tests COMMAND verify_tests.exe)
    else()
        add_test(NAME verify_tests COMMAND verify_tests)
    endif()
endif()
