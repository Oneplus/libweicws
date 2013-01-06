SET ( CQDB_SOURCE_DIR
    ${CRFSUITE_DIR}/lib/cqdb/src )

SET ( CRF_SOURCE_DIR
    ${CRFSUITE_DIR}/lib/crf/src )

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

ADD_LIBRARY (cqdb ${CQDB_SOURCES} )
ADD_LIBRARY (crfsuite ${CRFSUITE_SOURCES} )

TARGET_LINK_LIBRARIES (crfsuite lbfgs)
