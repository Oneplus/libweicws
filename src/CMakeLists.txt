INCLUDE_DIRECTORIES (
    ../include;
    ./;
    ./utils/)
#    ${THIRDPARTY_PATH}/boost;
#    ${THIRDPARTY_PATH}/crfsuite/include;)

LINK_DIRECTORIES (
    ../lib)

ADD_LIBRARY(weicws;
    ./weicws.cpp;
    ./core.cpp;
    ./encode.cpp;
    ./utils/ul_strlib_x.cpp)

TARGET_LINK_LIBRARIES(weicws;
    boost_regex
    cqdb
    crfsuite)

