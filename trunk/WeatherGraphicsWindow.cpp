
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

const QFont KCurrentLocationFont("Arial",20);
const QFont KCurrentTemperatureFont("Arial",40);
const QFont KLowHighTemperatureFont("Arial",12);
const QFont KCurrentConditionFont("Arial",17);
const QFont KWindowFont("Arial narrow",170);

WeatherGraphicsWindow::WeatherGraphicsWindow(QObject* aParent)
{
qDebug() << "WeatherGraphicsWindow::WeatherGraphicsWindow->";

	// Create signal mapper
	iSignalMapper = new QSignalMapper(this);
	iSignalMapper->setObjectName(KSignalMapperObjectName);
	qDebug()<<"Signalmapper obj name:"<<iSignalMapper->objectName();

	iNetworkEngine = new WeatherWidgetNetworkEngine(this);
	qDebug() << "WeatherGraphicsWindow::WeatherGraphicsWindow created network engine";
	connect(iNetworkEngine,SIGNAL(weatherRequestCompleted(const WeatherResponseT&,RequestMethodT,bool)),
			this,SLOT(on_networkEngine_weatherRequestCompleted(const WeatherResponseT&,RequestMethodT,bool)));

	iSelectLocation = new SelectLocation;
	
	createWeatherItems();
	
	// Auto connect signals and slots
	QMetaObject::connectSlotsByName(this);

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
	this->setPreferredHeight(qreal(220));
	adjustSize();
	
	iVerticalViewLayout = new QGraphicsLinearLayout(Qt::Vertical);

	iVerticalCurrentTempContainer = new QGraphicsLinearLayout(Qt::Vertical,iVerticalViewLayout);
	iHorizontalCurrentTempLayout = new QGraphicsLinearLayout(Qt::Horizontal,iVerticalViewLayout);
		
	// Location
	iCurrentLocation = new WeatherGraphicsWidget();
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

	// Apply transforms
	QTransform transform_location;
	transform_location.rotate(7);
	transform_location.translate(0,20);
	
	QTransform transform_tmp;
	transform_tmp.rotate(12);
	
	QTransform transform_cond;
	transform_cond.translate(0,70);
	transform_cond.rotate(-6);
	
	iCurrentLocation->setTransform(transform_location);
	iCurrentTemperature->setTransform(transform_tmp);
	iCurrentCondition->setTransform(transform_cond);

	// Apply colors
	iCurrentLocation->setColor(QColor(Qt::blue));
	iCurrentTemperature->setColor(QColor(Qt::cyan));
	iCurrentCondition->setColor(QColor(Qt::yellow));
	// Add to layouts
	//iVerticalViewLayout->setSpacing(qreal(30));
	iVerticalViewLayout->addItem(iCurrentLocation);
	iVerticalViewLayout->setAlignment(iCurrentLocation,Qt::AlignCenter);
	iVerticalViewLayout->addItem(iCurrentTemperature);
	iVerticalViewLayout->setAlignment(iCurrentTemperature,Qt::AlignCenter);
	iVerticalViewLayout->addItem(iCurrentCondition);
	iVerticalViewLayout->setAlignment(iCurrentCondition,Qt::AlignCenter);
	
	setLayout(iVerticalViewLayout);
	
	this->updateGeometry();
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
qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped->";
	switch(aItemId)
	{
		case ECurrentLocation: 
		{
			qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped ECurrentLocation";
			iSelectLocation->show();
		}
		break;	
			
		case ECurrentTemperature:
		{
			qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped ECurrentTemperature";
			startWeatherDataUpdate();
		}
		break;
		case ECurrentTemperatureUnits:
		{
			qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped ECurrentTemperatureUnits";
			startWeatherDataUpdate();
		}
		break;
		case ECurrentCondition:
		{
			qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped ECurrentCondition";
			startWeatherDataUpdate();
		}
		break;
		
		default:
		{
			qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped default";
			startWeatherDataUpdate();
		}
		break;
	}
qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped<-";	
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
		iCurrentLocation->updateText(iCurrentLocationInfo.iCityName + "," + iCurrentLocationInfo.iCountryName);
		
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


void WeatherGraphicsWindow::paint(QPainter *painter, 
								  const QStyleOptionGraphicsItem *option,
           						  QWidget *widget)
{
	painter->save();
	
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setFont(KWindowFont);
	painter->setPen(QColor(Qt::lightGray));
	painter->drawText(rect().toAlignedRect(),Qt::AlignCenter,this->iCurrentWeatherInfo.iCurrentTemperature);
	
	painter->restore();
}

// eof