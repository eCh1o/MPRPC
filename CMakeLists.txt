#版本
cmake_minimum_required(VERSION 3.0)
project(mprpc)
#可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#库文件输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)
#库文件搜索路径 -L
link_directories(${PROJECT_SOURCE_DIR}/lib)

#src包含了mprpc框架所有相关代码
add_subdirectory(src)
#example包含了mprpc框架使用的实例代码
add_subdirectory(example)