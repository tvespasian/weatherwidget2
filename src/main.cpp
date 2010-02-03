#include <QtGui>
#include <QApplication>

#include "inc\WeatherGraphicsWindow.h"
#include "inc\WeatherWidgetGraphicsView.h"

// Comment when debug log is not required
//#define DEBUG_OUTPUT

#ifdef DEBUG_OUTPUT

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
	
    #ifdef DEBUG_OUTPUT
    // Prepare debug log
	logfile.setParent(&a);
	
    logfile.setFileName(KLogFileName);
    logfile.open(QIODevice::Append);
    qInstallMsgHandler(MyOutputHandler);
    
    #endif

    // used for qsettings
    qApp->setOrganizationName("decii");
    qApp->setApplicationName("WeatherWidget2");
    a.setWindowIcon(QIcon(":/resources/images/appicon.png"));
    
    WeatherWidgetGraphicsView panel;
    panel.setFocus();
    panel.show();

    // start event loop
    return a.exec();
}

// eof