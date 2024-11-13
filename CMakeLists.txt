cmake_minimum_required(VERSION 3.10)

# Install vcpkg
include(FetchContent)

FetchContent_Declare(
    vcpkg
    GIT_REPOSITORY https://github.com/microsoft/vcpkg
    GIT_TAG 2024.10.21
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/vcpkg
)
FetchContent_MakeAvailable(vcpkg)

set(CMAKE_TOOLCHAIN_FILE "${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")

project(webapi)

set(SRC
    src/HTTPServer.hpp
    src/HTTPServer.cpp
    src/base64.h
    src/base64.cpp
    src/main.cpp
    src/typedefs.hpp
    src/auth/BasicAuthenticator.hpp
    src/auth/BasicAuthenticator.cpp
    src/auth/IAuthenticator.hpp
    src/controllers/IController.hpp
    src/controllers/UserController.hpp
    src/controllers/UserController.cpp
    src/controllers/utils.hpp
    src/controllers/utils.cpp
    src/db/SQLiteBackend.hpp
    src/db/SQLiteBackend.cpp
    src/entities/User.hpp
    src/entities/serialization.cpp
    src/repositories/UserRepository.hpp
    src/repositories/UserRepository.cpp
    src/repositories/Repository.hpp
    src/repositories/Repository.cpp
    src/services/UserService.hpp
    src/services/UserService.cpp
)

add_executable(webapi ${SRC})

target_include_directories(webapi PRIVATE src)
target_compile_features(webapi PRIVATE cxx_std_23)

find_package(httplib CONFIG REQUIRED)
target_link_libraries(webapi PRIVATE httplib::httplib)

find_package(jwt-cpp CONFIG REQUIRED)
target_link_libraries(webapi PRIVATE jwt-cpp::jwt-cpp)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(webapi PRIVATE nlohmann_json::nlohmann_json)
set(nlohmann-json_IMPLICIT_CONVSIONS OFF)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(webapi PRIVATE unofficial::sqlite3::sqlite3)