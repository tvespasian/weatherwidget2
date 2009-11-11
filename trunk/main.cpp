#include "weatherwidget2.h"
#include "WeatherGraphicsWindow.h"
#include <QtGui>
#include <QApplication>

// Remove comment when no debug log is required
#define QT_NO_DEBUG_OUTPUT

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
    graphicsview.setVisible(false);
    graphicsview.setScene(&scene);
    graphicsview.setWindowTitle("WeatherWidget 2");
    graphicsview.setWindowOpacity(0.9);
    
    // This is to get transulent effect
    //graphicsview.setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    //graphicsview.setAttribute(Qt::WA_TranslucentBackground,true);
    //graphicsview.setAttribute(Qt::WA_NoSystemBackground,true);
    
    //graphicsview.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //graphicsview.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
 /* WeatherWidget2 weatherwidget;
    weatherwidget.ui.verticalLayout->addWidget(&graphicsview);
    weatherwidget.show();*/
    
    graphicsview.setBackgroundBrush(QBrush(QColor(Qt::gray)));
    graphicsview.setMaximumHeight(320);
    graphicsview.setMinimumWidth(240);
    
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