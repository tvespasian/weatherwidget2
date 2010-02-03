#ifndef WEATHERWIDGETNETWORKENGINE_H_
#define WEATHERWIDGETNETWORKENGINE_H_

#include <QHttp>
#include <QXmlStreamReader>
#include <QHostAddress>
#include <QNetworkInterface>

enum RequestMethodT
{
ENone,	
EForecast,
ELiveRSS,
ECompactLiveRSS,
ESearchLocationFromString,
ELocalTime
};

enum StationTypeT
{
ECitycode, // Non US Cities
EZipcode // US Cities
};

struct WeatherResponseT
{
	QString iResponseBuffer;
	QString iStatusDescription;
	int iStatusCode;
	
public:
	void clear()
	{
		iResponseBuffer.clear();
		iStatusDescription.clear();
		iStatusCode = -1;
	}
};

//object name, use the following object names to let auto signal to slot connection
const QString KNetworkEngineObjectName("networkengine");

class WeatherWidgetNetworkEngine:public QObject
{
	Q_OBJECT
	
public:
	WeatherWidgetNetworkEngine(QObject* aParent=0);
	~WeatherWidgetNetworkEngine();

public:
	QList<QHostAddress> getAddresses()
	{
		return QNetworkInterface::allAddresses(); 
	}
	
	// network access methods
	void getStationCode(const QString& aLocation);

	void getWeatherInfo(const QString& aStationCode,
						const StationTypeT& aStationType,
						bool UnitsInC,
					    RequestMethodT aRequestMethod);
	
	void getLocalTime(const QString& aLatitude,
			  		  const QString& aLongitude);	
public slots:
	// QHttp
	void on_networkengine_requestStarted(int aId);
	void on_networkengine_requestFinished(int aId, bool aError);
	void on_networkengine_readyRead(const QHttpResponseHeader& resp);
	void on_networkengine_done(bool aError);
	
signals:
	void weatherRequestStarted();
	void weatherRequestCompleted(const WeatherResponseT& aResponse,
								 RequestMethodT aRequestMethod,
							  	 bool aError);
	
private:	
	int iCurrentReqId; // http request id, provided by QHttp
	QHttp* iHttp;
	QXmlStreamReader iXml;
	WeatherResponseT iHttpResponse;
	RequestMethodT iCurrentReqMethod;
};

#endif 
