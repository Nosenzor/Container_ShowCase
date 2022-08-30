#Add debug compile
cmake_minimum_required(VERSION 3.16)
project(Containers_tests DESCRIPTION "small show case for Threading Building Block" LANGUAGES CXX)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(SRC_FILES test_containers_main.cc containers_test.cc)

#BOOST setup
set(BOOST_ROOT /prj/qct/sve/dragonr/tools/boost_1_73_0/)
set(Boost_USE_STATIC_LIBS  ON)
find_package(Boost 1.73.0 REQUIRED COMPONENTS unit_test_framework container)


include_directories(${TBB_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

#Create unit test exe for the connectivity_extractor
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}  )

