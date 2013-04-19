INCLUDE_DIRECTORIES ( internal utils )

LINK_DIRECTORIES (${LIBRARY_OUTPUT_PATH}/)

SET (weicws_SRC core.cpp
    core.h
    weicws.cpp
    encode.cpp
    utils/ul_strlib_x.cpp)

ADD_LIBRARY(weicws SHARED ${weicws_SRC})
ADD_LIBRARY(weicws_STATIC STATIC ${weicws_SRC})

TARGET_LINK_LIBRARIES(weicws
    pcre
    pcrecpp
    cqdb
    crfsuite)

TARGET_LINK_LIBRARIES(weicws_STATIC
    pcre
    pcrecpp
    cqdb
    crfsuite)

set_target_properties (weicws_STATIC PROPERTIES OUTPUT_NAME weicws)

