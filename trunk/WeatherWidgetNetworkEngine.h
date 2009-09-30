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
ESearchLocationFromString
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
const QString KHttpObjectName("httpobject");
const QString KObjectName("networkEngine");

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
	void updateWeatherInfo(const QString& aCityName,
					   const QString& aCountryName,
					   RequestMethodT requestType);

	void getWeatherInfo(const QString& aStationCode,
					   RequestMethodT aRequestMethod = ECompactLiveRSS);
public slots:
	// QHttp
	void on_httpobject_requestStarted(int aId);
	void on_httpobject_requestFinished(int aId, bool aError);
	void on_httpobject_readyRead(const QHttpResponseHeader& resp);
	void on_httpobject_done(bool aError);
	
signals:	
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

#endif /*WEATHERWIDGETNETWORKENGINE_H_*/
