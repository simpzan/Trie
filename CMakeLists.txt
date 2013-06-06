# variables
set(PROJECT_NAME Trie)

cmake_minimum_required(VERSION 2.6)
project(${PROJECT_NAME})

# include and lib dirs
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${DAC_SOURCE_DIR}/src)
include_directories(${utils_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/../DAC/src)
include_directories(${PROJECT_SOURCE_DIR}/../utils/src)

link_directories(${PROJECT_SOURCE_DIR}/../DAC/build)
link_directories(${PROJECT_SOURCE_DIR}/../utils/build)

# lib target
set(LIB_NAME ${PROJECT_NAME})
file(GLOB SRC src/*.cpp)
add_library(${LIB_NAME} ${SRC})

# unit test target
set(UNIT_TEST_NAME unit_test_${PROJECT_NAME})
file(GLOB SRC_TEST test/*.cpp)
add_executable(${UNIT_TEST_NAME} ${SRC_TEST})
target_link_libraries(${UNIT_TEST_NAME} pthread gtest gtest_main)
target_link_libraries(${UNIT_TEST_NAME} Trie utils DAC)

# tool targets
macro(addtools tool_dir)
  file(GLOB SRC_TOOL ${tool_dir}/*.cpp)
  foreach(tool_file ${SRC_TOOL})
    get_filename_component(tool_name ${tool_file} NAME_WE)
    add_executable(${tool_name} ${tool_file})
    target_link_libraries(${tool_name} Trie utils leveldb)
  endforeach(tool_file)
endmacro(addtools)
addtools(tool)
