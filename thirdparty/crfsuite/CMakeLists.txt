PROJECT( CRFSUITE )

SET(PROJECT_BINARY_DIR ${SOURCE}/internal)


SET(CQDB_SOURCE_DIR ${CRFSUITE_DIR}/lib/cqdb/src )
SET(CRF_SOURCE_DIR  ${CRFSUITE_DIR}/lib/crf/src )

IF (MSVC)
    INCLUDE_DIRECTORIES ( ${LBFGS_DIR}/include
        ${CRFSUITE_DIR}/include
        ${CRFSUITE_DIR}/lib/cqdb/include
        ${THIRDPARTY_DIR}/win32 )
ELSE (MSVC)
    INCLUDE_DIRECTORIES ( ${LBFGS_DIR}/include
        ${CRFSUITE_DIR}/include
        ${CRFSUITE_DIR}/lib/cqdb/include )
ENDIF (MSVC)

SET (CQDB_SOURCES
    ${CQDB_SOURCE_DIR}/cqdb.c
    ${CQDB_SOURCE_DIR}/lookup3.c)

SET (CRFSUITE_SOURCES
    ${CRF_SOURCE_DIR}/crf1d.h
    ${CRF_SOURCE_DIR}/crf1d_context.c
    ${CRF_SOURCE_DIR}/crf1d_encode.c
    ${CRF_SOURCE_DIR}/crf1d_feature.c
    ${CRF_SOURCE_DIR}/crf1d_model.c
    ${CRF_SOURCE_DIR}/crf1d_tag.c
    ${CRF_SOURCE_DIR}/crfsuite.c
    ${CRF_SOURCE_DIR}/crfsuite_internal.h
    ${CRF_SOURCE_DIR}/crfsuite_train.c
    ${CRF_SOURCE_DIR}/dataset.c
    ${CRF_SOURCE_DIR}/dictionary.c
    ${CRF_SOURCE_DIR}/holdout.c
    ${CRF_SOURCE_DIR}/logging.c
    ${CRF_SOURCE_DIR}/logging.h
    ${CRF_SOURCE_DIR}/params.c
    ${CRF_SOURCE_DIR}/params.h
    ${CRF_SOURCE_DIR}/quark.c
    ${CRF_SOURCE_DIR}/quark.h
    ${CRF_SOURCE_DIR}/rumavl.c
    ${CRF_SOURCE_DIR}/rumavl.h
    ${CRF_SOURCE_DIR}/train_arow.c
    ${CRF_SOURCE_DIR}/train_averaged_perceptron.c
    ${CRF_SOURCE_DIR}/train_l2sgd.c
    ${CRF_SOURCE_DIR}/train_lbfgs.c
    ${CRF_SOURCE_DIR}/train_passive_aggressive.c
    ${CRF_SOURCE_DIR}/vecmath.h )

ADD_LIBRARY (cqdb SHARED ${CQDB_SOURCES} )
ADD_LIBRARY (cqdb_STATIC STATIC ${CQDB_SOURCES} )
set_target_properties (cqdb_STATIC PROPERTIES OUTPUT_NAME cqdb)

ADD_LIBRARY (crfsuite SHARED ${CRFSUITE_SOURCES} )
ADD_LIBRARY (crfsuite_STATIC STATIC ${CRFSUITE_SOURCES} )
set_target_properties (crfsuite_STATIC PROPERTIES OUTPUT_NAME crfsuite)

TARGET_LINK_LIBRARIES (crfsuite lbfgs cqdb)
#TARGET_LINK_LIBRARIES (crfsuite_STATIC lbfgs_STATIC cqdb_STATIC)

CONFIGURE_FILE(${CRFSUITE_DIR}/include/crfsuite.h
    ${PROJECT_BINARY_DIR}/crfsuite.h)
CONFIGURE_FILE(${CRFSUITE_DIR}/lib/cqdb/include/cqdb.h 
    ${PROJECT_BINARY_DIR}/cqdb.h)

MESSAGE(STATUS "")
MESSAGE(STATUS "[PASSED] CRFSUITE IS CONFIGED")
MESSAGE(STATUS "")


