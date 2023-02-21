greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


HEADERS += \
    src/fontmanager.h \
    src/gamescene.h \
    src/pixmapmanager.h \
    src/polyomino.h \
    src/resource_holder.h \
    src/utils.h \
    src/view.h \

SOURCES += \
    src/fontmanager.cpp \
    src/gamescene.cpp \
    src/main.cpp \
    src/pixmapmanager.cpp \
    src/polyomino.cpp \
    src/view.cpp \

RESOURCES += \
    resource.qrc