TEMPLATE = app
TARGET = WeatherWidget2
QT += core \
    gui \
    xml \
    xmlpatterns \
    network \
    webkit
HEADERS += SelectLocation.h \
    WeatherWidgetXMLEngine.h \
    WeatherWidgetNetworkEngine.h \
    WeatherGraphicsWidget.h \
    WeatherGraphicsWindow.h \
    weatherwidget2.h
SOURCES += SelectLocation.cpp \
    WeatherWidgetXMLEngine.cpp \
    WeatherWidgetNetworkEngine.cpp \
    WeatherGraphicsWidget.cpp \
    WeatherGraphicsWindow.cpp \
    main.cpp \
    weatherwidget2.cpp
FORMS += SelectLocation.ui \
    weatherwidget2.ui
RESOURCES += 
//DEFINES += QT_NO_DEBUG_OUTPUT
