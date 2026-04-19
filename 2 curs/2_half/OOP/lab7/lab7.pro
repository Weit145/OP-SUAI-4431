QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    apartmentcalc.cpp \
    calculationfacade.cpp \
    cottagecalc.cpp \
    estate.cpp \
    estatefactory.cpp \
    luxuriousapartmentcalc.cpp \
    main.cpp \
    montheighteen.cpp \
    monthsix.cpp \
    periodfactory.cpp \
    states.cpp \
    townhousecalc.cpp \
    widget.cpp \
    yearone.cpp

HEADERS += \
    abstractcalc.h \
    apartmentcalc.h \
    calcfactory.h \
    calculationfacade.h \
    cottagecalc.h \
    estate.h \
    estatefactory.h \
    luxuriousapartmentcalc.h \
    montheighteen.h \
    monthsix.h \
    periodfactory.h \
    states.h \
    townhousecalc.h \
    widget.h \
    yearone.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
