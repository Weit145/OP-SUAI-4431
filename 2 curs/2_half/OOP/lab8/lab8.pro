QT += widgets

CONFIG += c++17

SOURCES += \
    agencyfacade.cpp \
    commissionstrategy.cpp \
    entities.cpp \
    entityfactory.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    agencyfacade.h \
    commissionstrategy.h \
    entities.h \
    entityfactory.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
