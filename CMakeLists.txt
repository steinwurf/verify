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

add_subdirectory("${STEINWURF_RESOLVE}/libassert" libassert)

add_library(${PROJECT_NAME} STATIC src/verify.cpp)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
                                                    )
if (NOT NDEBUG)
    target_include_directories(${PROJECT_NAME} INTERFACE ${STEINWURF_RESOLVE}/libassert-source/include)
    add_dependencies(${PROJECT_NAME} libassert::assert)
    target_link_libraries(${PROJECT_NAME} libassert::assert)
endif()



if (${PROJECT_IS_TOP_LEVEL})
    add_executable(${PROJECT_NAME}_example example.cpp)
    target_link_libraries(${PROJECT_NAME}_example ${PROJECT_NAME})
    target_compile_features(${PROJECT_NAME}_example PRIVATE cxx_std_17)
endif()
