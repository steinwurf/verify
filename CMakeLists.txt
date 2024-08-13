cmake_minimum_required(VERSION 3.12)

project(
    verify
    VERSION 0.1.0
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
    message(STATUS "STEINWURF_VERIFY_USE_LIBASSERT not defined, defaulting to YES")
    set(STEINWURF_VERIFY_USE_LIBASSERT YES)
endif()

add_library(${PROJECT_NAME} STATIC src/verify.cpp)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE include)
add_library(steinwurf::verify ALIAS ${PROJECT_NAME})

install(
    DIRECTORY ./include
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    FILES_MATCHING
    PATTERN *.hpp)


if (NOT STEINWURF_VERIFY_NO_LIBASSERT)
    if (NOT TARGET steinwurf::libassert)
        add_subdirectory("${STEINWURF_RESOLVE}/libassert" libassert)
    endif()

    target_link_libraries(${PROJECT_NAME} PUBLIC steinwurf::libassert)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LIBASSERT_PREFIX_ASSERTIONS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC STEINWURF_VERIFY_USE_LIBASSERT)
endif()

if (${PROJECT_IS_TOP_LEVEL})
    add_executable(${PROJECT_NAME}_example example.cpp)
    target_link_libraries(${PROJECT_NAME}_example steinwurf::verify steinwurf::libassert)
    target_compile_features(${PROJECT_NAME}_example PRIVATE cxx_std_17)
endif()
