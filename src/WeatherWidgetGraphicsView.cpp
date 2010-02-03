
#include "inc\WeatherWidgetGraphicsView.h"
#include "inc\WeatherGraphicsWindow.h"
#include "inc\common.h"

WeatherWidgetGraphicsView::WeatherWidgetGraphicsView()
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground,true);
	setAttribute(Qt::WA_ContentsPropagated,true);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(FullViewportUpdate);
    setRenderHints(QPainter::Antialiasing | 
    			   QPainter::SmoothPixmapTransform | 
    		       QPainter::TextAntialiasing);

    // transparent background	
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Base, Qt::transparent);
    setPalette(palette);
    setAttribute(Qt::WA_OpaquePaintEvent, false); // trasmit color 
	setFrameStyle(QFrame::NoFrame);
	
    iScene = new QGraphicsScene(this);
	setScene(iScene);
	resize(KWindowWidth+KExtraStrech,KWindowHeight+KExtraStrech);
	
	// Create main window which holds weather data
	iMainWindow = new WeatherGraphicsWindow(this);
	iScene->addItem(iMainWindow);
	iMainWindow->setZValue(0);
	iMainWindow->setObjectName("mainwindow");
	connect(iMainWindow,SIGNAL(minimize()),
			this,SLOT(on_mainwindow_minimize()));
	connect(iMainWindow,SIGNAL(temperatureUpdated(const QString&)),
			this,SLOT(setWindowTitle(const QString&)));
	
}

WeatherWidgetGraphicsView::~WeatherWidgetGraphicsView()
{
qDebug()<<"WeatherWidgetGraphicsView::~WeatherWidgetGraphicsView->";
	if(iScene)
	{
		delete iScene;
	}
qDebug()<<"WeatherWidgetGraphicsView::~WeatherWidgetGraphicsView<-";	
}

void WeatherWidgetGraphicsView::on_mainwindow_minimize()
{
	showMinimized();
}

//eof
