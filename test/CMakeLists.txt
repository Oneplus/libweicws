INCLUDE_DIRECTORIES ( ./  ../include/)

LINK_DIRECTORIES ( ${LIBRARY_OUTPUT_PATH}/ )

ADD_EXECUTABLE(weicws-test demo.cpp)

TARGET_LINK_LIBRARIES( weicws-test weicws
    pcre
    pcrecpp
    cqdb
    crfsuite
    lbfgs)
