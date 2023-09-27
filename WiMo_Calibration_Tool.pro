QT += core gui
QT += qml quick
#QT += charts

#-- Ajout Perso --
QT += serialport
QT += core
QT += quickcontrols2
QT += printsupport
QT -= opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O3

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O0

CONFIG += qwt

contains( COMPILATION_VERSION, 32 ) {
    # drivers contains 'network'
    #message( "Compilation 32 bits" )
    QMAKEFEATURES = C:\Qwt-6.4.0-svn-32-15.1\features
    DEFINE += "COMPILATION_VERSION=32"
}
contains( COMPILATION_VERSION, 64 ) {
    # drivers contains 'network'
    #message( "Compilation 64 bits" )
    QMAKEFEATURES = C:\Qwt-6.4.0-svn-64-15.1\features
    DEFINES += "COMPILATION_VERSION=64"
}

LIBS += -lKernel32

#Application version
VERSION_MAJOR = 1
VERSION_MINOR = 3
VERSION_BUILD = 6
VERSION_REVISION = 2

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\
           "VERSION_BUILD=$$VERSION_BUILD"\
           "VERSION_REVISION=$$VERSION_REVISION"

#Target version
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}.$${VERSION_REVISION}

#Target name
#TEMPLATE = WiMo_Calibration_Tool_V$$VERSION
contains( COMPILATION_VERSION, 64 ) {
 contains( aDEBUG, 1 ) {
  TARGET = WiMo_Calibration_Tool_V$${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}.$${VERSION_REVISION}-x64-debug
 }
 contains( aDEBUG, 0 ) {
  TARGET = WiMo_Calibration_Tool_V$${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}.$${VERSION_REVISION}-x64
 }
 INCLUDEPATH += C:/Qwt-6.4.0-svn-64-15.1/include
 INCLUDEPATH += ./Composants/Ftdi-64
 LIBS += -L$$PWD/Composants/Ftdi-64 -lftd2xx
 #LIBS += -L$$PWD/Qwt-64
# LIBS += -L"C:\Qwt-6.4.0-svn-64-15.1\lib"-lqwt
 include(C:/Qwt-6.4.0-svn-64-15.1/features/qwt.prf)
}
contains( COMPILATION_VERSION, 32 ) {
 contains( aDEBUG, 1 ) {
  TARGET = WiMo_Calibration_Tool_V$${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}.$${VERSION_REVISION}-x86-debug
 }
 contains( aDEBUG, 0 ) {
  TARGET = WiMo_Calibration_Tool_V$${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}.$${VERSION_REVISION}-x86
 }
 INCLUDEPATH += C:/Qwt-6.4.0-svn-32-15.1/include
 INCLUDEPATH += ./Composants/Ftdi-32
 LIBS += -L$$PWD/Composants/Ftdi-32 -lftd2xx
 #LIBS += -L$$PWD/Liveview/Qwt-32
 #LIBS += -L$$PWD/Ftdi/ -lftd2xx
 #LIBS += -L"C:\Qwt-6.4.0-svn-32-15.1\lib"-lqwt
 include(C:/Qwt-6.4.0-svn-32-15.1/features/qwt.prf)
}
# Emplacement du target file
DESTDIR = ../Factory/Exe

INCLUDEPATH += ./
INCLUDEPATH += ./Calibration
INCLUDEPATH += ./Composants/DateTime
INCLUDEPATH += ./Composants/Endian
INCLUDEPATH += ./Composants/FIFO
INCLUDEPATH += ./Composants/Ftdi-32
INCLUDEPATH += ./Composants/Ftdi-64
INCLUDEPATH += ./Composants/Modbus
INCLUDEPATH += ./Composants/TcpIp
INCLUDEPATH += ./Configuration
INCLUDEPATH += ./Dashboard
INCLUDEPATH += ./Factory
INCLUDEPATH += ./Liveview
INCLUDEPATH += ./Modbus
INCLUDEPATH += ./Target
INCLUDEPATH += ./Target/UART
INCLUDEPATH += ./Type
INCLUDEPATH += ./Update
INCLUDEPATH += ./WIMO
INCLUDEPATH += ./Windows
INCLUDEPATH += ./Windows/WindowsParamDisplay

#2020.09.15 - Enleve
#RC_FILE = Logo/myapp.rc
# TEST ENLEVE
#win32:RC_ICONS += Logo/fav2_Low.ico
RC_ICONS = Logo/fav2_re.ico
QMAKE_TARGET_COMPANY = nke
QMAKE_TARGET_PRODUCT = WiMo Calibration Tool
QMAKE_TARGET_COPYRIGHT = nke Instrumentation
#contains( COMPILATION_VERSION, 32 ) {
#QMAKE_TARGET_DESCRIPTION = "Build with Qt 5.15.1 MinGw 32-bit"
#}
#contains( COMPILATION_VERSION, 64 ) {
#QMAKE_TARGET_DESCRIPTION = "Build with Qt 5.15.1 MinGw 64-bit"
#}

#-----------------

#QMAKE_CXXFLAGS += "-Wno-old-style-cast"
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Calibration/calibpoint.cpp \
        Calibration/calibration.cpp \
        Composants/DateTime/DATETIMELib_4_05.cpp \
        Composants/FIFO/FIFODriver_4_02.cpp \
        Composants/Ftdi-32/stdafx.cpp \
        Composants/Ftdi-64/stdafx.cpp \
        Composants/Modbus/FIFOModBusDriver_2_04.cpp \
        Composants/Modbus/ModBusDriver_2_04.cpp \
        Dashboard/DashboardControler.cpp \
        DataCom.cpp \
        DataSensor.cpp \
        DataSensorCT.cpp \
        DataSensorChannel.cpp \
        Factory/factoryevent.cpp \
        Liveview/LiveViewControler.cpp \
        Liveview/liveview.cpp \
        Liveview/liveviewconfevent.cpp \
        Liveview/plotdata.cpp \
        Liveview/qwtquick2.cpp \
        MainData.cpp \
        Modbus/ModbusControler.cpp \
        Modbus/ModbusInterfaceUpdateControler.cpp \
        TINIConfig.cpp \
        Target/UART/readserialport.cpp \
        Update/FileDownloader.cpp \
        Update/GetHTTPFile.cpp \
        Update/HTTPUpdate.cpp \
        Update/InterfaceUpdateControler.cpp \
        Update/InterfaceUpdateData.cpp \
        Update/SensorUpdateControler.cpp \
        Update/SensorUpdateData.cpp \
        WIMO/WIMOClientModbus.cpp \
        WIMO/WIMOParameter.cpp \
        WIMO/WIMOSensor.cpp \
        WIMO/o2event.cpp \
        Windows/WindowDebugEvent.cpp \
        Windows/WindowsParamDisplay/customtype.cpp \
        Windows/WindowsParamDisplay/treeitem.cpp \
        Windows/WindowsParamDisplay/treemodel.cpp \
        Windows/windowcomevent.cpp \
        closeevent.cpp \
        main.cpp \
        mainevent.cpp \
        wiper.cpp

RESOURCES += \
    qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains( COMPILATION_VERSION, 32 ) {
 HEADERS += \
 Composants/Ftdi-32/ftd2xx.h \
 Composants/Ftdi-32/stdafx.h \
 Composants/Ftdi-32/targetver.h
}
contains( COMPILATION_VERSION, 64 ) {
 HEADERS += \
 Composants/Ftdi-64/ftd2xx.h \
 Composants/Ftdi-64/stdafx.h \
 Composants/Ftdi-64/targetver.h
}

HEADERS += \
    Calibration/calibpoint.h \
    Calibration/calibration.h \
    Composants/DateTime/DATETIMELibConfiguration_4_05.h \
    Composants/DateTime/DATETIMELib_4_05.h \
    Composants/Endian/Endian_1_00.h \
    Composants/FIFO/FIFODriverConfiguration_4_02.h \
    Composants/FIFO/FIFODriver_4_02.h \
    Composants/Modbus/FIFOModBusDriver_2_04.h \
    Composants/Modbus/ModBusData_2_04.h \
    Composants/Modbus/ModBusDriver_2_04.h \
    Composants/TcpIp/PILETopoPhysique_1_20.h \
    Composants/TcpIp/PILETypes_1_20.h \
    Configuration/FIFOModBusDriverConfiguration_2_04.h \
    Configuration/ModBusDataConfiguration_2_04.h \
    Configuration/ModBusDriverConfiguration_2_04.h \
    Dashboard/DashboardControler.h \
    DataCom.h \
    DataSensor.h \
    DataSensorCT.h \
    DataSensorChannel.h \
    Factory/factoryevent.h \
    Liveview/LiveViewControler.h \
    Liveview/liveview.h \
    Liveview/liveviewconfevent.h \
    Liveview/plotdata.h \
    Liveview/qwtquick2.h \
    MainData.h \
    Modbus/FIFOModBusDriver_2_04.h \
    Modbus/ModBusData_2_04.h \
    Modbus/ModBusDriver_2_04.h \
    Modbus/ModbusControler.h \
    Modbus/ModbusInterfaceUpdateControler.h \
    TINIConfig.h \
    Target/TARGETHardware.h \
    Target/UART/readserialport.h \
    Type/TypeC.h \
    Type/TypeCMMN.h \
    Type/TypeMicro.h \
    UART/readserialport.h \
    Update/FileDownloader.h \
    Update/GetHTTPFile.h \
    Update/HTTPUpdate.h \
    Update/InterfaceUpdateControler.h \
    Update/InterfaceUpdateData.h \
    Update/SensorUpdateControler.h \
    Update/SensorUpdateData.h \
    WIMO/WIMOClientModBus.h \
    WIMO/WIMOMath.h \
    WIMO/WIMOParameters.h \
    WIMO/WIMOSensor.h \
    WIMO/liveview.h \
    main.h \
    WIMO/o2event.h \
    Windows/WindowDebugEvent.h \
    Windows/WindowsParamDisplay/customtype.h \
    Windows/WindowsParamDisplay/treeitem.h \
    Windows/WindowsParamDisplay/treemodel.h \
    Windows/windowcomevent.h \
    calibpoint.h \
    calibration.h \
    closeevent.h \
    dashboard.h \
    dashboardhandle.h \
    factoryevent.h \
    factoryevent.h \
    graphData.h \
    liveview.h \
    liveviewconfevent.h \
    mainevent.h \
    plotdata.h \
    qmlcallback.h \
    qwtquick2.h \
    windowcomevent.h \
    wiper.h

DISTFILES += \
    Composants/Ftdi-32/ftd2xx.lib \
    Composants/Ftdi-64/ftd2xx.lib \
    Font/OpenSans-Bold.ttf \
    Font/OpenSans-BoldItalic.ttf \
    Font/OpenSans-ExtraBold.ttf \
    Font/OpenSans-ExtraBoldItalic.ttf \
    Font/OpenSans-Italic.ttf \
    Font/OpenSans-Light.ttf \
    Font/OpenSans-LightItalic.ttf \
    Font/OpenSans-Regular.ttf \
    Font/OpenSans-Semibold.ttf \
    Font/OpenSans-SemiboldItalic.ttf \
    Readme.md \
    fav2_re.ico

################
# Traduction
###############
TRANSLATIONS = wimo_calibration_tool_fr.ts \
               wimo_calibration_tool_es.ts \
               wimo_calibration_tool_de.ts

#LIBS += -L$$PWD/Ftdi/ -lftd2xx

#INCLUDEPATH += $$PWD/Ftdi
#DEPENDPATH += $$PWD/Ftdi
