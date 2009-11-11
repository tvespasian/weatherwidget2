#include "weatherwidget2.h"

WeatherWidget2::WeatherWidget2(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
    show();
}

WeatherWidget2::~WeatherWidget2()
{
}


// eof