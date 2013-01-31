INCLUDE_DIRECTORIES ( ./  ../include/)

LINK_DIRECTORIES ( ${LIBRARY_OUTPUT_PATH}/ )

ADD_EXECUTABLE(demo demo.cpp)
ADD_EXECUTABLE(multi_demo multi_demo.cpp)

TARGET_LINK_LIBRARIES( demo weicws
    pcre
    pcrecpp
    cqdb
    crfsuite)

