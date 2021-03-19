cmake_minimum_required(VERSION 3.15)

project(CppTutorials)

# 每个变量一行的形式方便run.py进行读取
SET(
        EXAMPLES
        RValueRef
        Memory
        DefaultConstructor
        CopyConstructor
)

foreach (EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.cpp)
endforeach ()

