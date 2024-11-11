cmake_minimum_required(VERSION 3.10)

# Install vcpkg
include(FetchContent)

FetchContent_Declare(
    vcpkg
    GIT_REPOSITORY https://github.com/microsoft/vcpkg
    GIT_TAG 2024.10.21
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/vcpkg
)
FetchContent_MakeAvailable(vcpkg)

set(CMAKE_TOOLCHAIN_FILE "${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")

project(webapi)

set(SRC
    src/main.cpp
    src/repositories/SQLiteRepository.cpp
)

add_executable(webapi ${SRC})

target_include_directories(webapi PRIVATE src)
target_compile_features(webapi PRIVATE cxx_std_23)

find_package(httplib CONFIG REQUIRED)
target_link_libraries(webapi PRIVATE httplib::httplib)

find_package(jwt-cpp CONFIG REQUIRED)
target_link_libraries(webapi PRIVATE jwt-cpp::jwt-cpp)

set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(webapi PRIVATE nlohmann_json::nlohmann_json)