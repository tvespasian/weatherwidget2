#include "weatherwidget2.h"
#include <QtGui>
#include <QApplication>

// Comment this when no debug log is required
//#define QT_NO_DEBUG_OUTPUT

#ifndef QT_NO_DEBUG_OUTPUT

const QString KLogFileName("logfile.log");
QFile logfile;

// This is the methid to handle all debug msgs
void MyOutputHandler(QtMsgType , const char *);

void MyOutputHandler(QtMsgType type, const char *msg) 
{
	if(QApplication::instance()->closingDown() || false == logfile.isOpen())
	{
		return;
	}
	
	
	QString logmsg;
	logmsg += QTime::currentTime().toString() + " " + msg + "\n";

	switch (type) 
    {
        case QtDebugMsg:
        {
        	logmsg.insert(0,"Debug: ");
        }
            break;
        case QtCriticalMsg:
        {
        	logmsg.insert(0,"Critical: ");
        }
            break;
        case QtWarningMsg:
        {
        	logmsg.insert(0,"Warning: ");
        }
            break;
        case QtFatalMsg:
        {
        	logmsg.insert(0,"Fatal: ");
            abort();
        }
            break;
    }
    
    logfile.write(logmsg.toAscii());
    logfile.flush();
}
#endif

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
    #ifndef QT_NO_DEBUG_OUTPUT
    // Prepare debug log
	logfile.setParent(&a);
	
    logfile.setFileName(KLogFileName);
    logfile.open(QIODevice::Append);
    qInstallMsgHandler(MyOutputHandler);
    
    #endif

// this is for test    
#define xx
    
#ifdef xx    
    WeatherGraphicsWindow* weatherwindow = new WeatherGraphicsWindow(&a);
    
    QGraphicsScene scene;
    
	scene.addItem(weatherwindow);
	    
    QGraphicsView graphicsview;
    graphicsview.setScene(&scene);
    graphicsview.setWindowTitle("WeatherWidget 2");
    graphicsview.setWindowOpacity(0.9);
    	
    //graphicsview.setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    
    graphicsview.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsview.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    graphicsview.setBackgroundBrush(QBrush(QColor(Qt::gray)));
    graphicsview.resize(400,300);
    graphicsview.show();
    graphicsview.update();
    
#else if    
    WeatherWidget2 weatherwidget;
    weatherwidget.show();
#endif 
    // start event loop
    return a.exec();
}

// eof