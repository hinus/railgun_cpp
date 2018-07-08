PROJECT(RAILGUN)

SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -ggdb ")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

ADD_EXECUTABLE(railgun main.cpp
    klass/klass.cpp
    object/hiTypeObject.cpp
	object/hiNoneObject.cpp
	util/bufferedInputStream.cpp
	util/arrayList.cpp
	code/binaryFileParser.cpp
	code/codeObject.cpp)
	
INCLUDE_DIRECTORIES(./)
