INCLUDE_DIRECTORIES ( internal utils )

LINK_DIRECTORIES (
    ../lib)

ADD_LIBRARY(weicws STATIC
    ./core.cpp 
    ./core.h
    ./weicws.cpp
    ./encode.cpp
    ./utils/ul_strlib_x.cpp)

TARGET_LINK_LIBRARIES(weicws
    pcre
    pcrecpp
    cqdb
    crfsuite)
