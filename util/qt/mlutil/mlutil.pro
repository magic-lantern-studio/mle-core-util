QT -= gui

TEMPLATE = lib
DEFINES += MLUTIL_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += MLE_NOT_DLL

INCLUDEPATH += $$PWD/../../common/include $$PWD/../../linux/include

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/../../common/src/mlDebug.c \
    $$PWD/../../common/src/MleDsoLoader.cxx \
    $$PWD/../../common/src/MleMemoryManager.cxx \
    $$PWD/../../common/src/mlErrno.c \
    $$PWD/../../common/src/MleTemplate.cxx \
    $$PWD/../../common/src/mlExpandFilename.c \
    $$PWD/../../common/src/mlItoa.c \
    $$PWD/../../common/src/mlLogFile.c \
    $$PWD/../../common/src/mlUnique.c \
    $$PWD/../../linux/src/MleLinuxMemoryManager.cxx \
    $$PWD/../../linux/src/MleLinuxPath.cxx


HEADERS += \
    $$PWD/../../common/include/mle/mlArray.h \
    $$PWD/../../common/include/mle/mlAssert.h \
    $$PWD/../../common/include/mle/mlConfig.h \
    $$PWD/../../common/include/mle/mlDebug.h \
    $$PWD/../../common/include/mle/MleDsoLoader.h \
    $$PWD/../../common/include/mle/mle.h \
    $$PWD/../../common/include/mle/MleMemoryManager.h \
    $$PWD/../../common/include/mle/MlePath.h \
    $$PWD/../../common/include/mle/mlErrno.h \
    $$PWD/../../common/include/mle/MleTemplate.h \
    $$PWD/../../common/include/mle/MleUtil.h \
    $$PWD/../../common/include/mle/mlExpandFilename.h \
    $$PWD/../../common/include/mle/mlFileio.h \
    $$PWD/../../common/include/mle/mlItoa.h \
    $$PWD/../../common/include/mle/mlLogFile.h \
    $$PWD/../../common/include/mle/mlMacros.h \
    $$PWD/../../common/include/mle/mlMalloc.h \
    $$PWD/../../common/include/mle/mlToken.h \
    $$PWD/../../common/include/mle/mlTypes.h \
    $$PWD/../../common/include/mle/mlUnique.h


# Default rules for deployment.
unix {
    target.path = /opt/MagicLantern/lib
    headers.path = /opt/MagicLantern/include/mle
    headers.files = $$HEADERS
    INSTALLS += target headers
}
!isEmpty(target.path): INSTALLS += target
