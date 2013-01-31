INCLUDE_DIRECTORIES ( ./
    ../include/
    ./thirdparty/tinythreadpp/ )

LINK_DIRECTORIES ( ${LIBRARY_OUTPUT_PATH}/ )

ADD_EXECUTABLE(demo demo.cpp)

TARGET_LINK_LIBRARIES( demo weicws
    pcre
    pcrecpp
    cqdb
    crfsuite)

ADD_EXECUTABLE(multi_demo 
    multi_demo.cpp
    ./thirdparty/tinythreadpp/tinythread.cpp)

IF (NOT WIN32)
    TARGET_LINK_LIBRARIES( multi_demo weicws pthread )
ELSE()
    TARGET_LINK_LIBRARIES( multi_demo weicws )
ENDIF()

