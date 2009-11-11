#ifndef WEATHERWIDGET2_H
#define WEATHERWIDGET2_H

#include <QtGui/QMainWindow>
#include "ui_weatherwidget2.h"

class WeatherWidget2 : public QMainWindow
{
    Q_OBJECT

public:
    WeatherWidget2(QWidget *parent = 0);
    ~WeatherWidget2();

public:
    Ui::WeatherWidget2Class ui;
};

#endif // WEATHERWIDGET2_H
