INCLUDE_DIRECTORIES (
    ./
    ../include/)

LINK_DIRECTORIES (
    ../lib)

ADD_EXECUTABLE(demo;
    ./weicws.cpp)

TARGET_LINK_LIBRARIES(demo;
    weicws)
