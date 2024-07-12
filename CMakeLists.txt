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
    FetchContent_MakeAvailable(libassert)
endif()


add_executable(example example.cpp)
add_executable(steinwurf::example ALIAS example)
target_link_libraries(example PRIVATE libassert::assert)
target_compile_features(example PRIVATE cxx_std_20)
target_compile_definitions(example PRIVATE LIBASSERT_PREFIX_ASSERTIONS)

# TODO: Remove these from the top level.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

install(TARGETS example)
