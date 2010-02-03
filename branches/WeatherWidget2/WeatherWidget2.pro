TEMPLATE = app
TARGET = WeatherWidget2
QT += core \
    gui \
    xml \
    xmlpatterns \
    network \
    webkit
HEADERS += inc/GraphicsWidgetItem.h \
    inc/SelectLocation.h \
    inc/Settings.h \
    inc/WeatherDialog.h \
    inc/WeatherForecastWidget.h \
    inc/WeatherGraphicsWidget.h \
    inc/WeatherGraphicsWindow.h \
    inc/WeatherWidgetGraphicsView.h \
    inc/WeatherWidgetNetworkEngine.h \
    inc/WeatherWidgetXMLEngine.h \
    inc/common.h
SOURCES += src/GraphicsWidgetItem.cpp \
    src/SelectLocation.cpp \
    src/Settings.cpp \
    src/WeatherDialog.cpp \
    src/WeatherForecastWidget.cpp \
    src/WeatherGraphicsWidget.cpp \
    src/WeatherGraphicsWindow.cpp \
    src/WeatherWidgetGraphicsView.cpp \
    src/WeatherWidgetNetworkEngine.cpp \
    src/WeatherWidgetXMLEngine.cpp \
    src/main.cpp
FORMS += SelectLocation.ui
RESOURCES += WeatherWidget2.qrc
//DEFINES += QT_NO_DEBUG_OUTPUT
