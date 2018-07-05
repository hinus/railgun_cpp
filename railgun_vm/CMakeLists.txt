PROJECT(RAILGUN)

ADD_EXECUTABLE(railgun main.cpp
	util/bufferedInputStream.cpp
	code/binaryFileParser.cpp
	code/codeObject.cpp)
	
INCLUDE_DIRECTORIES(./)
