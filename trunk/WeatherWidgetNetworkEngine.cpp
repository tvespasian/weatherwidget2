
#include "WeatherWidgetNetworkEngine.h"
#include <QMetaObject>

// weatherbug api methods
const QString KMethodCompactLiveRSS("/getLiveCompactWeatherRSS.aspx");
const QString KMethodSearchLocation("/getLocationsXML.aspx");

// POST parameters
const QString KSearchLocationsParam("ACode=A5580663883&SearchString="); // append search string before use
const QString KCompactLiveRSSParam("ACode=A5580663883&unittype=1&outputtype=1&citycode=");

const QString KACode("A5580663883"); // srikanth's first acode
const QString KWebServer("www.api.wxbug.net"); //weatherbug api server


const QString KLocationXmlError("Sorry! The requested location is not found.");
const QString KProductName("WeatherWidget");

const int KDegC = 176;
const QString KWindowIconImage("images/windowiconimage.png");
const QString KSysTrayIcon("images/systrayimage.png");
const QString KWBImage("images/hail.png");

const int KCityNameYPos = -120;
const int KCurrTempYPos = -100;
const int KCurrConditionYPos = 100;

// timers
const int KUpdateIntervalInMs = 1200000; // 20 mins
const int KSysTrayMsgTimeoutInMs = 5000; // 5 secs

//50,35
const int KBGImageXPos = 50;
const int KBGImageYPos = 35;

enum ErrorT
{
	KErrNone = 0,
	KErrNotFound = -1	
};

WeatherWidgetNetworkEngine::WeatherWidgetNetworkEngine(QObject* aParent)
{
	setObjectName(KObjectName);
	
	// Create it with parent, coz we send this parent for auto signal connection
	iHttp = new QHttp(this);
	
	iHttp->setObjectName(KHttpObjectName);
	
	// Auto connecting Signals and slots
	// This checks for child objects, so pass the parent instance
	QMetaObject::connectSlotsByName(this);
	
}

WeatherWidgetNetworkEngine::~WeatherWidgetNetworkEngine()
{
qDebug() << "WeatherWidgetNetworkEngine::~WeatherWidgetNetworkEngine->";
	iHttp->abort();
	iHttp->close();
	iHttpResponse.clear();
qDebug() << "WeatherWidgetNetworkEngine::~WeatherWidgetNetworkEngine<-";	
}

void WeatherWidgetNetworkEngine::on_httpobject_requestStarted(int aId)
{
	iHttpResponse.clear();
}

void WeatherWidgetNetworkEngine::on_httpobject_requestFinished(int id, bool error)
{
	
}

void WeatherWidgetNetworkEngine::on_httpobject_readyRead(const QHttpResponseHeader& resp)
{
qDebug()<<"WeatherWidgetNetworkEngine::on_httpobject_readyRead->";

	iHttpResponse.iResponseBuffer.append(iHttp->readAll());
	iHttpResponse.iStatusCode = resp.statusCode();
	
qDebug()<<"WeatherWidgetNetworkEngine::on_httpobject_readyRead<-";
}

void WeatherWidgetNetworkEngine::on_httpobject_done(bool aError)
{
qDebug()<<"WeatherWidgetNetworkEngine::handleDone->";
qDebug()<<"error"<<aError;
qDebug()<<iHttpResponse.iResponseBuffer;

	iHttpResponse.iStatusDescription = iHttp->errorString();
	emit weatherRequestCompleted(iHttpResponse,iCurrentReqMethod,aError);

qDebug()<<"WeatherWidgetNetworkEngine::handleDone<-";	
}

void WeatherWidgetNetworkEngine::updateWeatherInfo(const QString& aCityName,
											   const QString& aCountryName,
											   RequestMethodT requestType)
{
qDebug()<<"WeatherWidgetNetworkEngine::getStationCode->";

	if(0 == aCityName.length())
	{
		return;
	}
	
	iCurrentReqMethod = requestType;
	
	// clear pending and current requests
	iHttp->abort();
	
	//all requests are to the same server
	iHttp->setHost(KWebServer);

	// Clear http response buffer
	iHttpResponse.clear();
	
	QString params(KSearchLocationsParam);
	params += aCityName;
	if(aCountryName.length())
	{
		params += ","+aCountryName;
	}
	
	qDebug()<<"post params for getstationcode "<<params;
	QHttpRequestHeader header("POST",KMethodSearchLocation);
	header.setValue("Host", KWebServer);
	header.setContentType("application/x-www-form-urlencoded");
	iCurrentReqId = iHttp->request(header,params.toUtf8());
	
qDebug()<<"WeatherWidgetNetworkEngine::getStationCode<-";	
}

void WeatherWidgetNetworkEngine::getWeatherInfo(const QString& aStationCode,
					   						   RequestMethodT aRequestMethod)
{
qDebug()<<"WeatherWidgetNetworkEngine::getWeatherInfo->";

	iHttpResponse.clear();
	
	// clear pending and current requests
	iHttp->abort();
	
	//all requests are to the same server
	iHttp->setHost(KWebServer);
	
	// prepare post request
	qDebug()<<"Preparing POST Params for ";
	iCurrentReqMethod = aRequestMethod;
	switch(iCurrentReqMethod)
	{
	case ECompactLiveRSS:
		{
		qDebug()<<"ECompactLiveRSS";	
		QString params(KCompactLiveRSSParam);
		params.append(aStationCode);
		QHttpRequestHeader header("POST",KMethodCompactLiveRSS);
		header.setValue("Host", KWebServer);
		header.setContentType("application/x-www-form-urlencoded");
		iHttp->request(header,params.toUtf8());
		}
		break;
	}	
qDebug()<<"WeatherWidgetNetworkEngine::getWeatherInfo<-";
}


// eof