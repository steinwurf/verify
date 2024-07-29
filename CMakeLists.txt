cmake_minimum_required(VERSION 3.12)

project(
    verify
    VERSION 0.1.0
    DESCRIPTION "a meta assertion library focused on high compatibility"
    HOMEPAGE_URL "https://github.com/steinwurf/verify"
    LANGUAGES CXX
)

if(STEINWURF_VERIFY_USE_EXTERNAL_LIBASSERT)
    find_package(libassert REQUIRED)
else()
    include(FetchContent)
    FetchContent_Declare(
      libassert
      GIT_REPOSITORY https://github.com/jeremy-rifkin/libassert.git
      GIT_TAG        v2.1.0
    )
endif()

#add_library(verify INTERFACE)

add_executable(example example.cpp)
add_executable(steinwurf::example ALIAS example)
set_property(TARGET example PROPERTY CXX_STANDARD 17)

# TODO: Do as a function?
get_target_property(example_CXX_STANDARD example CXX_STANDARD)
if(example_CXX_STANDARD VERSION_GREATER_EQUAL 17.0.0)
    # Error (We only support C++>=17). Libassert has it.
    message("Building with libassert support...")
    FetchContent_MakeAvailable(libassert)
    target_link_libraries(example PRIVATE libassert::assert)
    # We only need the implementation of libassert, tell libassert to not create any ASSERT prefixes for us.
    target_compile_definitions(example PRIVATE LIBASSERT_PREFIX_ASSERTIONS)
endif()

# TODO: Remove these from the top level.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

install(TARGETS example)
