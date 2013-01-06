INCLUDE_DIRECTORIES ( ./  ../include/)

LINK_DIRECTORIES ( ${LIBRARY_OUTPUT_PATH}/ )

ADD_EXECUTABLE(demo weicws.cpp)

TARGET_LINK_LIBRARIES( demo weicws
    pcre
    pcrecpp
    cqdb
    crfsuite)
