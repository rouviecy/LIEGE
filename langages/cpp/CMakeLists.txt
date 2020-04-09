cmake_minimum_required(VERSION 2.8)
project(LIEGE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(LIEGE_LIB
	src/model/Node.cpp		include/model/Node.h
	src/model/Edge.cpp		include/model/Edge.h
	src/model/Graph.cpp		include/model/Graph.h
)

add_library(TINYXML_LIB
	src/TinyXML/tinyxml.cpp		include/TinyXML/tinyxml.h
	src/TinyXML/tinystr.cpp		include/TinyXML/tinystr.h
	src/TinyXML/tinyxmlerror.cpp
	src/TinyXML/tinyxmlparser.cpp
)

add_executable(test src/test.cpp)
target_link_libraries(test LIEGE_LIB TINYXML_LIB stdc++fs)