
#include "inc\WeatherWidgetNetworkEngine.h"
#include <QMetaObject>

// weatherbug api methods
const QString KMethodCompactLiveRSS("/getLiveCompactWeatherRSS.aspx");
const QString KMethodLiveRSS("/getLiveWeatherRSS.aspx"); // extended info than compact live
const QString KMethodForecastRSS("/getForecastRSS.aspx");
const QString KMethodSearchLocation("/getLocationsXML.aspx");

// parameters
const QString KSearchLocationsParam("ACode=A5580663883&SearchString="); // append search string before use
const QString KWeatherRequestParamsInC("ACode=A5580663883&outputtype=1&unittype=1&");
const QString KWeatherRequestParamsInF("ACode=A5580663883&outputtype=1&unittype=0&");
const QString KWeathetRequestCitycodeParam("citycode=");
const QString KWeathetRequestZipcodeParam("zipcode=");

// geonames.org api methods
const QString KGeoNamesTimeZone("/timezone");
const QString KGeoNamesParamsLatLong("lat=%1&lng=%2");

// Parameters

const QString KACode("A5580663883"); // srikanth's first acode
const QString KWebServer("www.api.wxbug.net"); //weatherbug api server
const QString KGeoNamesWebServer("ws.geonames.org"); // Geonames.org 

const QString KLocationXmlError("Sorry! The requested location is not found.");
//const QString KProductName("WeatherWidget");

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
	
	// Create it with parent, coz we send this parent for auto signal connection
	iHttp = new QHttp(this);
	
	iHttp->setObjectName(KNetworkEngineObjectName);
	
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

void WeatherWidgetNetworkEngine::on_networkengine_requestStarted(int aId)
{
qDebug()<<"WeatherWidgetNetworkEngine::on_networkengine_requestStarted->";
	emit weatherRequestStarted();
	iHttpResponse.clear();
qDebug()<<"WeatherWidgetNetworkEngine::on_networkengine_requestStarted<-";	
}

void WeatherWidgetNetworkEngine::on_networkengine_requestFinished(int id, bool error)
{
	
}

void WeatherWidgetNetworkEngine::on_networkengine_readyRead(const QHttpResponseHeader& resp)
{
qDebug()<<"WeatherWidgetNetworkEngine::on_networkengine_readyRead->";

	iHttpResponse.iResponseBuffer.append(iHttp->readAll());
	iHttpResponse.iStatusCode = resp.statusCode();
	
qDebug()<<"WeatherWidgetNetworkEngine::on_networkengine_readyRead<-";
}

void WeatherWidgetNetworkEngine::on_networkengine_done(bool aError)
{
qDebug()<<"WeatherWidgetNetworkEngine::handleDone->";
qDebug()<<"error"<<aError;
qDebug()<<iHttpResponse.iResponseBuffer;

	iHttpResponse.iStatusDescription = iHttp->errorString();
	emit weatherRequestCompleted(iHttpResponse,iCurrentReqMethod,aError);

qDebug()<<"WeatherWidgetNetworkEngine::handleDone<-";	
}

void WeatherWidgetNetworkEngine::getStationCode(const QString& aLocation)
{
qDebug()<<"WeatherWidgetNetworkEngine::getStationCode->";

	if(0 == aLocation.length())
	{
		return;
	}
	
	iCurrentReqMethod = ESearchLocationFromString;
	
	// clear pending and current requests
	iHttp->abort();
	
	//all requests are to the same server
	iHttp->setHost(KWebServer);

	// Clear http response buffer
	iHttpResponse.clear();
	
	QString params(KSearchLocationsParam);
	params += aLocation;
	
	qDebug()<<"params for getstationcode "<<params;
	QHttpRequestHeader header("GET",KMethodSearchLocation);
	header.setValue("Host", KWebServer);
	header.setContentType("application/x-www-form-urlencoded");
	iCurrentReqId = iHttp->request(header,params.toUtf8());
	
qDebug()<<"WeatherWidgetNetworkEngine::getStationCode<-";	
}

void WeatherWidgetNetworkEngine::getWeatherInfo(const QString& aStationCode,
												const StationTypeT& aStationType,
												bool unitsInC,
					   						    RequestMethodT aRequestMethod)
{
qDebug()<<"WeatherWidgetNetworkEngine::getWeatherInfo->";

	iHttpResponse.clear();
	
	// clear pending and current requests
	iHttp->abort();
	
	//all requests are to the same server
	iHttp->setHost(KWebServer);
	
	// prepare post request
	qDebug()<<"Preparing Params for ";
	iCurrentReqMethod = aRequestMethod;
	QHttpRequestHeader header;
	
	switch(iCurrentReqMethod)
	{
	case ECompactLiveRSS:
		{
			qDebug()<<"ECompactLiveRSS";
			header.setRequest("GET",KMethodCompactLiveRSS);
		}
		break;
		
	case ELiveRSS:
		{
			qDebug()<<"ELiveRSS";
			header.setRequest("GET",KMethodLiveRSS);
		}
		break;
		
	case EForecast:
		{
			qDebug()<<"EForecast";
			header.setRequest("GET",KMethodForecastRSS);
		}
		break;
	}	
	
	// append station code to request parameters
	QString params;
	// request in C
	if(unitsInC)
	{
		params = KWeatherRequestParamsInC;
	}
	// request in F
	else
	{
		params = KWeatherRequestParamsInF;
	}
	
	// append correct station type
	if(ECitycode == aStationType)
	{
		params += KWeathetRequestCitycodeParam;
	}
	
	else if(EZipcode == aStationType)
	{
		params += KWeathetRequestZipcodeParam;
	}
	params.append(aStationCode);
	
	header.setValue("Host", KWebServer);
	header.setContentType("application/x-www-form-urlencoded");
	qDebug()<<header.toString();
	iHttp->request(header,params.toUtf8());
	
qDebug()<<"WeatherWidgetNetworkEngine::getWeatherInfo<-";
}


void WeatherWidgetNetworkEngine::getLocalTime(const QString& aLatitude,
				  							  const QString& aLongitude)
{
qDebug()<<"WeatherWidgetNetworkEngine::getLocalTime->";

	iHttpResponse.clear();
	// clear pending and current requests
	iHttp->abort();
	iHttp->setHost(KGeoNamesWebServer);
	
	iCurrentReqMethod = ELocalTime;
	QString uri("http://ws.geonames.org/timezone?lat=%1&lng=%2");
	uri = uri.arg(aLatitude,aLongitude);
	qDebug()<<uri;
	iHttp->get(uri);
qDebug()<<"WeatherWidgetNetworkEngine::getLocalTime<-";
}


// eof