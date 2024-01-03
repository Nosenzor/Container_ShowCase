#Add debug compile
cmake_minimum_required(VERSION 3.16)

if (NOT CMAKE_TOOLCHAIN_FILE)
        message(STATUS "CMAKE_TOOLCHAIN_FILE not set, setting it to ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
                CACHE STRING "Vcpkg toolchain file")
endif()
message(STATUS "CMAKE_TOOLCHAIN_FILE : ${CMAKE_TOOLCHAIN_FILE}")

#Forcing VCPKG to only use release version
set(VCPKG_BUILD_TYPE release)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Add project_options
# https://github.com/aminya/project_options
include(FetchContent)
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
        cmake_policy(SET CMP0135 NEW)
endif()
set(PROJECT_OPTIONS_VERSION "v0.32.1")
FetchContent_Declare(
        _project_options
        URL https://github.com/aminya/project_options/archive/refs/tags/${PROJECT_OPTIONS_VERSION}.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

# install vcpkg dependencies: - should be called before defining project()
run_vcpkg(
        VCPKG_URL "https://github.com/microsoft/vcpkg.git"
)

# install vcpkg dependencies: - should be called before defining project()
#run_vcpkg()


project(training_containers DESCRIPTION "short training on containers" LANGUAGES CXX)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Initialize project_options variable related to this project
# This overwrites `project_options` and sets `project_warnings`
# uncomment to enable the options. Some of them accept one or more inputs:
project_options(
        PREFIX ${PROJECT_NAME}
        ENABLE_CACHE
        ${ENABLE_CPPCHECK}
        ${ENABLE_CLANG_TIDY}
        ENABLE_VS_ANALYSIS
        # ENABLE_CONAN
        ENABLE_INTERPROCEDURAL_OPTIMIZATION
        ENABLE_NATIVE_OPTIMIZATION
        ${ENABLE_DOXYGEN}
        ${ENABLE_COVERAGE}
        ${ENABLE_SANITIZER_ADDRESS}
        ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
        # ${ENABLE_SANITIZER_THREAD}
        # ${ENABLE_SANITIZER_MEMORY}
        # ENABLE_CONTROL_FLOW_PROTECTION
        # ENABLE_STACK_PROTECTION
        # ENABLE_OVERFLOW_PROTECTION
        # ENABLE_ELF_PROTECTION
        # ENABLE_RUNTIME_SYMBOLS_RESOLUTION
        # ENABLE_COMPILE_COMMANDS_SYMLINK
        # ENABLE_PCH
        # PCH_HEADERS
        # WARNINGS_AS_ERRORS
        # ENABLE_INCLUDE_WHAT_YOU_USE
        # ENABLE_GCC_ANALYZER
        # ENABLE_BUILD_WITH_TIME_TRACE
        # ENABLE_UNITY
        # LINKER "lld"
        # CONAN_PROFILE ${profile_path}
)

set(SRC_FILES test_containers_main.cc
              containers_test.cc)

#BOOST setup
#find_package(TBB REQUIRED)
find_package(Boost REQUIRED COMPONENTS unit_test_framework container)

#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)


#Create unit test exe for the connectivity_extractor
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS} ${TBB_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE training_containers_project_options training_containers_project_warnings
        Boost::unit_test_framework Boost::container
        )

enable_testing()
add_test(NAME containers COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/training_containers --log_level=all )
