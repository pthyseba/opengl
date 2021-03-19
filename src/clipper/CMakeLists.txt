PROJECT(polyclipping)

ADD_LIBRARY(polyclipping src/clipper.cpp)
TARGET_INCLUDE_DIRECTORIES(polyclipping PUBLIC include)
SET_TARGET_PROPERTIES(polyclipping PROPERTIES VERSION 22.0.0 SOVERSION 22 )
