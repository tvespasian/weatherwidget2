
#include <QtDebug>
#include <QFile>
#include <QIODevice>
#include "WeatherGraphicsWindow.h"
#include "WeatherWidgetXMLEngine.h"

#define QT_NO_DEBUG

// Item Ids
typedef enum 
{
	ECurrentLocation = 10,
	ECurrentTemperature,
	ECurrentTemperatureUnits,
	ECurrentCondition
}WeatherWidgetItems;

const QString KCurrentLocation("Current Location");
const QString KCurrentTemperature("Current Temperature");
const QString KCurrentTemperatureUnits("Current Temperature Units");
const QString KCurrentCondition("Current Condition");

//Default values
const QString KDefaultStationCode("64712");
const QString KDefaultCityName("Hyderabad");
const QString KDefaultCountryName("India");

const QFont KCurrentLocationFont("Arial",15);
const QFont KCurrentTemperatureFont("Arial",40);
const QFont KLowHighTemperatureFont("Arial",12);
const QFont KCurrentConditionFont("Arial",15);



WeatherGraphicsWindow::WeatherGraphicsWindow(QObject* aParent)
{
qDebug() << "WeatherGraphicsWindow::WeatherGraphicsWindow->";

	iNetworkEngine = new WeatherWidgetNetworkEngine(this);
	qDebug() << "WeatherGraphicsWindow::WeatherGraphicsWindow created network engine";

	// Auto connect signals and slots
	QMetaObject::connectSlotsByName(this);
	connect(iNetworkEngine,SIGNAL(weatherRequestCompleted(const WeatherResponseT&,RequestMethodT,bool)),
			this,SLOT(on_networkEngine_weatherRequestCompleted(const WeatherResponseT&,RequestMethodT,bool)));
	
	createWeatherItems();
	startWeatherDataUpdate();
	setOwnedByLayout(false);
	
qDebug() << "WeatherGraphicsWindow::WeatherGraphicsWindow<-";
}


WeatherGraphicsWindow::~WeatherGraphicsWindow()
{
qDebug() << "WeatherGraphicsWindow::~WeatherGraphicsWindow->";	
	if(iNetworkEngine)
	{
		delete iNetworkEngine;
	}
	
	if(iSignalMapper)
	{
		delete iSignalMapper;
	}
	
		
qDebug() << "WeatherGraphicsWindow::~WeatherGraphicsWindow<-";	
}


void WeatherGraphicsWindow::createWeatherItems()
{
qDebug() << "WeatherGraphicsWindow::createWeatherItems()->";

	// TODO : move to seperate method, this is used to get city name from ip addr.
	QList<QHostAddress> list = iNetworkEngine->getAddresses();
	
	for(int i=0;i<list.count();i++)
	{
		qDebug() << list[i].toString();
	}

	this->setPreferredWidth(qreal(400));
	this->setPreferredHeight(qreal(200));
	adjustSize();
	
	// Create signal mapper
	iSignalMapper = new QSignalMapper(this);
	iSignalMapper->setObjectName(KSignalMapperObjectName);
	qDebug()<<iSignalMapper->objectName();
	
	iVerticalViewLayout = new QGraphicsLinearLayout(Qt::Vertical,this);

	iVerticalCurrentTempContainer = new QGraphicsLinearLayout(Qt::Vertical,iVerticalViewLayout);
	iHorizontalCurrentTempLayout = new QGraphicsLinearLayout(Qt::Horizontal,iVerticalViewLayout);
		
	// Location
	iCurrentLocation = new WeatherGraphicsWidget(this);
	iCurrentLocation->setText("Hyderabad,India",KCurrentLocationFont);
	
	// connect with signal mapper
	connect(iCurrentLocation, SIGNAL(itemExecuted()), iSignalMapper, SLOT(map()));
	// Map to unique id 
	iSignalMapper->setMapping(iCurrentLocation,ECurrentLocation);
	
	iCurrentTemperature = new WeatherGraphicsWidget(this);
	iCurrentTemperature->setText("24.7",KCurrentTemperatureFont);
	//iCurrentTemperature->setAlignment(Qt::AlignRight);
	connect(iCurrentTemperature, SIGNAL(itemExecuted()), iSignalMapper, SLOT(map()));
	iSignalMapper->setMapping(iCurrentTemperature,ECurrentTemperature);
	
	/*iCurrentTemperatureUnits = new WeatherGraphicsWidget(this);
	iCurrentTemperatureUnits->setText("C");
	iCurrentTemperatureUnits->setAlignment(Qt::AlignLeft);
	connect(iCurrentTemperatureUnits, SIGNAL(itemExecuted()), iSignalMapper, SLOT(map()));
	iSignalMapper->setMapping(iCurrentTemperatureUnits,ECurrentTemperatureUnits);*/
	
	iCurrentCondition = new WeatherGraphicsWidget(this);
	iCurrentCondition->setText("this is a test condition",KCurrentConditionFont);
	connect(iCurrentCondition, SIGNAL(itemExecuted()), iSignalMapper, SLOT(map()));
	iSignalMapper->setMapping(iCurrentCondition,ECurrentCondition);

	// Add to layouts
	
	iVerticalViewLayout->addItem(iCurrentLocation);
	iVerticalViewLayout->addItem(iCurrentTemperature);
	iVerticalViewLayout->addItem(iCurrentCondition);
	
	setLayout(iVerticalViewLayout);
	
qDebug() << "WeatherGraphicsWindow::createWeatherItems()<-";	
}

void WeatherGraphicsWindow::startWeatherDataUpdate()
{
qDebug()<<"WeatherGraphicsWindow::fetchWeatherData->";


	// This will start weather data update loop
	iNetworkEngine->updateWeatherInfo(KDefaultCityName,KDefaultCountryName,ESearchLocationFromString);
	

qDebug()<<"WeatherGraphicsWindow::fetchWeatherData<-";
}

void WeatherGraphicsWindow::on_signalmapper_mapped(int aItemId)
{
	switch(aItemId)
	{
		case ECurrentLocation: 
		{
						
		}
		break;	
			
		case ECurrentTemperature:
		{
			
		}
		break;
		case ECurrentTemperatureUnits:
		{
			
		}
		break;
		case ECurrentCondition:
		{
			
		}
		break;
		
		default:
		{
			
		}
		break;
	}
	
}

void WeatherGraphicsWindow::on_networkEngine_weatherRequestCompleted(const WeatherResponseT& aResponse,
																	 RequestMethodT aRequestMethod,
						 				  			   			     bool aError)
{
qDebug()<<"WeatherGraphicsWindow::on_networkEngine_weatherRequestCompleted->";
qDebug()<<aResponse.iResponseBuffer;
qDebug()<<aResponse.iStatusDescription;
qDebug()<<aResponse.iStatusCode;

	if(aError)
	{
		qDebug()<<"Error: "<<aError;
		return;
	}
	
	if(ESearchLocationFromString == aRequestMethod)
	{
		QList<LocationInfoT> locationlist;
		bool stat = WeatherWidgetXMLEngine::getLocationDetails(aResponse.iResponseBuffer,locationlist);
		
		for(int i=0;i<locationlist.count();i++)
		{
			qDebug()<<i<<" "<<locationlist[i].iCityName<<" "<<locationlist[i].iCountryName<<" "<<locationlist[i].iCityCode;
		}
		
		// TODO : display list box for selection of multiple items
		iCurrentLocationInfo = locationlist[0];
		iCurrentLocation->updateText(iCurrentLocationInfo.iCityName + " " + iCurrentLocationInfo.iCountryName);
		
		iVerticalViewLayout->updateGeometry();
		
		qDebug()<<"after updating current location";
		qDebug()<<"iCurrentLocation "<<" w:"<<iCurrentLocation->rect().width()<<" h:"<<iCurrentLocation->rect().height();
		qDebug()<<"iVerticalViewLayout "<<" w:"<<iVerticalViewLayout->geometry().width()<<" h:"<<iVerticalViewLayout->geometry().height();
		
		
		iNetworkEngine->getWeatherInfo(locationlist[0].iCityCode,ECompactLiveRSS);
	}
	
	else if(ECompactLiveRSS == aRequestMethod)
	{
		bool stat = WeatherWidgetXMLEngine::getWeatherInfo(aResponse.iResponseBuffer,iCurrentWeatherInfo);
		qDebug()<<"temp: "<<iCurrentWeatherInfo.iCurrentTemperature<<" condition: "<<iCurrentWeatherInfo.iCurrentCondition;
		
		iCurrentTemperature->updateText(iCurrentWeatherInfo.iCurrentTemperature);
		iCurrentCondition->updateText(iCurrentWeatherInfo.iCurrentCondition);
		
		iVerticalViewLayout->updateGeometry();
		
		qDebug()<<"after updating current temp."; 
		qDebug()<<"iCurrentTemperature "<<" w:"<<iCurrentTemperature->rect().width()<<" h:"<<iCurrentTemperature->rect().height();
		qDebug()<<"iCurrentCondition "<<" w:"<<iCurrentCondition->rect().width()<<" h:"<<iCurrentCondition->rect().height();
		qDebug()<<"iVerticalViewLayout "<<" w:"<<iVerticalViewLayout->geometry().width()<<" h:"<<iVerticalViewLayout->geometry().height();
		qDebug()<<"iHorizontalCurrentTempLayout "<<" w:"<<iHorizontalCurrentTempLayout->geometry().width()<<" h:"<<iHorizontalCurrentTempLayout->geometry().height();

	}
	
	qDebug()<<"container window: w: "<<this->rect().width()<<"h: "<<this->rect().height();
	
qDebug()<<"WeatherGraphicsWindow::on_networkEngine_weatherRequestCompleted<-";	
}

// eof