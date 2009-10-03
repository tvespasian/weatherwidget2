#include "weatherwidget2.h"

WeatherWidget2::WeatherWidget2(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	
	iWeatherGraphicsWindow = new WeatherGraphicsWindow(this);
	
	QGraphicsScene scene;
	scene.addItem(iWeatherGraphicsWindow);
	    
    QGraphicsView* graphicsview = new QGraphicsView(ui.verticalLayoutWidget);
    graphicsview->setScene(&scene);
    graphicsview->setWindowTitle("WeatherWidget 2");
 
    // add to main window layout
    ui.verticalLayout->addWidget(graphicsview);
    
    this->show();
    //graphicsview->show();
    
}

WeatherWidget2::~WeatherWidget2()
{
	if(iWeatherGraphicsWindow)
	{
		delete iWeatherGraphicsWindow;
	}
}


// eof