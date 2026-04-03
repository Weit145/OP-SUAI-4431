QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    apartmentcalc.cpp \
    calculationfacade.cpp \
    cottagecalc.cpp \
    estate.cpp \
    luxuriousapartmentcalc.cpp \
    main.cpp \
    montheighteen.cpp \
    monthsix.cpp \
    states.cpp \
    townhousecalc.cpp \
    widget.cpp \
    yearone.cpp

HEADERS += \
    apartmentcalc.h \
    calculationfacade.h \
    cottagecalc.h \
    estate.h \
    luxuriousapartmentcalc.h \
    montheighteen.h \
    monthsix.h \
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
