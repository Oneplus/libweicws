PROJECT (libutlities)

SET(CMAKE_C_FLAGS           "$ENV{CXXFLAGS} -g -O3 -fPIC")
SET(CMAKE_CXX_FLAGS         "$ENV{CXXFLAGS} -g -O3 -Wno-deprecated -ftemplate-depth-50 -fPIC")

SET(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin)
SET(INCLUDE_OUTPUT_PATH     ${PROJECT_SOURCE_DIR}/include)
SET(LIBRARY_OUTPUT_PATH     ${PROJECT_SOURCE_DIR}/lib)
SET(SOURCE                  ${PROJECT_SOURCE_DIR}/src)

ADD_SUBDIRECTORY(src)

CONFIGURE_FILE(
    ${SOURCE}/weicws.h
    ${INCLUDE_OUTPUT_PATH}/weicws.h)

#CONFIGURE_FILE(
#    ${SOURCE}/string/token/ul_token.h
#    ${INCLUDE_OUTPUT_PATH}/ul_token.h)

#CONFIGURE_FILE(
#    ${SOURCE}/string/ul_strlib_x.h
#    ${INCLUDE_OUTPUT_PATH}/ul_strlib_x.h)

#CONFIGURE_FILE(
#    ${SOURCE}/logging/ul_logging_x.h
#    ${INCLUDE_OUTPUT_PATH}/ul_logging_x.h)

ADD_SUBDIRECTORY(test)
