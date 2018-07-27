cmake_minimum_required(VERSION 2.8)

PROJECT(RAILGUN)

SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -ggdb ")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

ADD_EXECUTABLE(railgun main.cpp
    klass/klass.cpp
    object/hiObject.cpp
    object/hiInteger.cpp
    object/hiNoneObject.cpp
    object/hiString.cpp
    object/hiList.cpp
    object/hiDict.cpp
    runtime/functionObject.cpp
    runtime/frameObject.cpp
    runtime/universe.cpp
    runtime/generator.cpp
    runtime/stringTable.cpp
    memory/heap.cpp
    memory/oopClosure.cpp
    util/bufferedInputStream.cpp
    util/arrayList.cpp
    util/map.cpp
    util/stack.cpp
    util/handles.cpp
    code/binaryFileParser.cpp
    code/codeObject.cpp
    code/interpreter.cpp)
    
INCLUDE_DIRECTORIES(./)
