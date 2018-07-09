cmake_minimum_required(VERSION 2.8)

PROJECT(RAILGUN)

SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -ggdb ")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

ADD_EXECUTABLE(railgun main.cpp
    klass/klass.cpp
	object/hiObject.cpp
	object/hiInteger.cpp
    object/hiTypeObject.cpp
	object/hiNoneObject.cpp
	object/hiString.cpp
	runtime/functionObject.cpp
	runtime/frameObject.cpp
	util/bufferedInputStream.cpp
	util/arrayList.cpp
	code/binaryFileParser.cpp
	code/codeObject.cpp
	code/interpreter.cpp)
	
INCLUDE_DIRECTORIES(./)
