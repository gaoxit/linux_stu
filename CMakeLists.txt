cmake_minimum_required(VERSION 3.5)

project(my_linux)
set(CMAKE_C_COMPILER gcc)
# set(CMAKE_C_COMPILER arm-linux-gnueabi-gcc) #添加该编译器后生成的文件是二进制文件
# gnueabi:
# gnu代表GNU is Not Unix（我理解就是一个自由软件标准），e代表的是嵌入式，abi是指Application Binary Interface，也就是二进制应用程序接口

link_directories(Binary)

add_compile_options(-g -O2 -ffunction-sections -fdata-sections -funwind-tables -Wall -Wextra -Werror -Wno-pointer-sign -Wno-unused)

FILE (GLOB ALL_SOURCES   
        "./src/*.c"
        "./src/myprintf/*.c"
)

include_directories(
        "./src/"
        "./src/myprintf"
)

SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections")

# include_directories(${CMAKE_CURRENT_BINARY_DIR})

execute_process(COMMAND date +LINUX_%y%m%d_%H%M_V1.01
    OUTPUT_VARIABLE GCC_VER OUTPUT_STRIP_TRAILING_WHITESPACE)

# 注释：
# 1.OUTPUT_STRIP_TRAILING_WHITESPACE:删除字符串尾的换行符
# 2.git describe返回git的节点和tag信息；--abbrev=10指定commit号显示10个字符；--dirty如果当前commit之后有新的提交(本地有修改不提交也会dirty)则最后加-dirty表示
execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND git describe --always --abbrev=10 --dirty
    OUTPUT_VARIABLE SHA_VER OUTPUT_STRIP_TRAILING_WHITESPACE)

#将ASCII码转化成对应的字符，此处ASCII码为27对应的字符为ESC，此外string还可以有一些其他功能，如TOLOWER(大写->小写)
string(ASCII 27 Escape)

message("${Escape}[33m${GCC_VER}${Escape}[0m")
message("${Escape}[33m${SHA_VER}${Escape}[0m")

execute_process(COMMAND date
    OUTPUT_VARIABLE cur_time)
message("${Escape}[3m${cur_time}${Escape}[0m")

configure_file(version.h version.h)

add_executable(my_linux.bin ${ALL_SOURCES})            #生成可执行文件，第一个参数为生成的可执行文件名，第二个为源文件的路径

# 使用pthread库
target_link_libraries(my_linux.bin pthread)