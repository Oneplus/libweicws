IF (MSVC)
    INCLUDE_DIRECTORIES ( ${LBFGS_DIR}/include 
        ${THIRDPARTY_DIR}/win32 )
ELSE (MSVC)
    INCLUDE_DIRECTORIES ( ${LBFGS_DIR}/include )
ENDIF (MSVC)


SET( LBFGS_SOURCE_DIR ${LBFGS_DIR}/lib )
SET( LBFGS_SOURCES
    ${LBFGS_SOURCE_DIR}/arithmetic_ansi.h
    ${LBFGS_SOURCE_DIR}/arithmetic_sse_double.h
    ${LBFGS_SOURCE_DIR}/arithmetic_sse_float.h
    ${LBFGS_SOURCE_DIR}/lbfgs.c )

ADD_LIBRARY( lbfgs ${LBFGS_SOURCES} )

