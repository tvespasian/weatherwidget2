
#include <QtDebug>
#include <QFile>
#include <QIODevice>
#include <QGraphicsProxyWidget>
#include <QGraphicsAnchorLayout>
#include <QGraphicsAnchor>
#include <QMessageBox>
#include <QImage>
#include <QPixmapCache>
#include <QDesktopServices>

#include "inc\WeatherGraphicsWindow.h"
#include "inc\WeatherForecastWidget.h"
#include "inc\WeatherWidgetXMLEngine.h"
#include "inc\common.h"
#include "inc\GraphicsWidgetItem.h"
#include "inc\SelectLocation.h"
#include "inc\Settings.h"

#define QT_NO_DEBUG

//#define _ONE_LAYOUT
//#define _APPLY_TRANSFORM

//#define USE_GRAPHIC_ITEMS

// Item Ids
typedef enum 
{
	ECurrentLocation = 10,
	ECurrentTemperature,
	ECurrentTemperatureUnits,
	ECurrentCondition,
	EClose,
	EMinimize,
	ESettings,
	EFavorites,
	EMaps,
	EHelp
}WeatherWidgetItems;

const QFont KCurrentLocationFont(DefaultFontName,(7*(KWindowWidth/2))/100);
const QFont KCurrentLocationLevel2Font(DefaultFontName,(7*(KWindowWidth/2))/100);
const QFont KCurrentLocationLevel3Font(DefaultFontName,(5*(KWindowWidth/2))/100);
const QFont KCurrentTemperatureFont(DefaultFontName,(28*(KWindowWidth/2))/100); 
const QFont KTemperatureUnitsFont(DefaultFontName,(20*(KWindowWidth/2))/100);
const QFont KCurrentConditionFont(DefaultFontName,(7*(KWindowWidth/2))/100);
const QFont KStatusTextFont(DefaultFontName,(5*(KWindowWidth/2))/100);

const QString KCurrentLocation("Current Location");
const QString KCurrentTemperature("Current Temperature");
const QString KCurrentTemperatureUnits("Current Temperature Units");
const QString KCurrentCondition("Current Condition");

//Default values
const QString KDefaultStationCode("64712");
const QString KDefaultCityName("Hyderabad");
const QString KDefaultCountryName("India");
const int KUpdateIntervalInMs = 1200000; // 20 mins
const int KLocalTimeUpdateIntervalInMs = 1000; // 1 minute
const QString KImageMappingXMLPath("imagemapping.xml");

const QString KGoogleMapsRequest("http://maps.google.com/maps?q=%1,%2+(%3)&iwloc=A&hl=en");

WeatherGraphicsWindow::WeatherGraphicsWindow(QObject* aParent)
					  :iMainWindowAnimationRequired(true)
{
qDebug() << "WeatherGraphicsWindow::WeatherGraphicsWindow->";
	
	setOwnedByLayout(true);

	// read settings
	Settings settings;
	settings.readLocationSettings(iCurrentLocationInfo);
	qDebug()<<"Restored last settings";
	qDebug()<<"Cityname:"<<iCurrentLocationInfo.iCityName<<"\nCitycode"<<iCurrentLocationInfo.iCityCode<<"\nZipcode:"<<iCurrentLocationInfo.iZipCode;
	
	// This is the first time app is running
	// Load hardcoded defaults
	if(iCurrentLocationInfo.iCityCode.isEmpty() && iCurrentLocationInfo.iZipCode.isEmpty())
	{
	iCurrentLocationInfo.iCityCode = KDefaultStationCode;
	iCurrentLocationInfo.iCityName = KDefaultCityName;
	iCurrentLocationInfo.iCountryName = KDefaultCountryName;
	}

	// read preferences
	WeatherWidgetPreferences pref;
	settings.readPreferences(pref);
	
	(pref.iUnits.isEmpty())?(iCurrentWeatherInfo.iUnits = KCelcius):(iCurrentWeatherInfo.iUnits = pref.iUnits);
	(0 == pref.iTimeout)?(iUpdateIntervalInMs = KUpdateIntervalInMs):(iUpdateIntervalInMs = pref.iTimeout);
	
	// Override parent property of transparent background
	// set non transparent background	
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Base, Qt::white);
    setPalette(palette);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
	resize(KWindowWidth,KWindowHeight);
	
	// Create signal mapper
	iSignalMapper = new QSignalMapper(this);
	iSignalMapper->setObjectName(KSignalMapperObjectName);

	// Create network engine
	iNetworkEngine = new WeatherWidgetNetworkEngine(this);
	iNetworkEngine->setObjectName(KNetworkEngineObjectName);
	connect(iNetworkEngine,SIGNAL(weatherRequestCompleted(const WeatherResponseT&,RequestMethodT,bool)),
			this,SLOT(on_networkengine_weatherRequestCompleted(const WeatherResponseT&,RequestMethodT,bool)));
	connect(iNetworkEngine,SIGNAL(weatherRequestStarted()),
			this,SLOT(on_networkengine_weatherRequestStarted()));

	// Create location selection dialog 
	iSelectLocation = new SelectLocation;
	iSelectLocation->setWindowFlags(Qt::FramelessWindowHint);
	connect(iSelectLocation->ui.pushButton_ok,SIGNAL(clicked()),
			this,SLOT(selectlocation_okclicked()));
	connect(iSelectLocation->ui.pushButton_cancel,SIGNAL(clicked()),
			iSelectLocation,SLOT(close()));

	// Create detailed forecast dialog
	iForecastDialog = new WeatherDialog(QSize(KWindowWidth-(KWindowWidth/4),KWindowHeight/3),this);
	iForecastDialog->setZValue(KHighestZValue);
	iForecastDialog->setPos(this->rect().center());

	iStatusDialog = new WeatherDialog(QSize(KWindowWidth-(KWindowWidth/4),KWindowHeight/3),this);
	iStatusDialog->setCloseOnClick(true);
	
	//
	// By this step all the required widgets are created and connected
	
	// Now create and add graphicswidgets
	createAndAddWeatherItems();
	
	// Auto connect signals and slots
	QMetaObject::connectSlotsByName(this);

	// Get first weather info
	getWeatherUpdate(iCurrentLocationInfo,ELiveRSS);
	// start timer, this will trigger weather update on every timeout
	iTimerIdWeatherupdate = startTimer(iUpdateIntervalInMs);
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

	// Save settings
	Settings settings;
	settings.writeLocationSettings(iCurrentLocationInfo);
	WeatherWidgetPreferences pref;
	pref.iTimeout = iUpdateIntervalInMs;
	pref.iUnits = iCurrentWeatherInfo.iUnits;
	settings.writePreferences(pref);
	
qDebug() << "WeatherGraphicsWindow::~WeatherGraphicsWindow<-";	
}

void WeatherGraphicsWindow::createAndAddWeatherItems()
{
qDebug() << "WeatherGraphicsWindow::createAndAddWeatherItems->";

	// Layout for all temperature widgets
	iHorizontalCurrentTempLayout = new QGraphicsLinearLayout(Qt::Horizontal);
	
	// Location
	iCurrentLocation = new WeatherGraphicsWidget("",KCurrentLocationFont,Qt::AlignLeft);
	iCurrentLocation->setToolTip("Click to change location");
	iCurrentLocationLevel2 = new WeatherGraphicsWidget("",KCurrentLocationLevel2Font,Qt::AlignLeft);
	iCurrentLocationLevel3 = new WeatherGraphicsWidget("",KCurrentLocationLevel3Font,Qt::AlignLeft);
	// connect to signal mapper
	connect(iCurrentLocation, SIGNAL(itemExecuted()), iSignalMapper, SLOT(map()));
	// Map to unique id 
	iSignalMapper->setMapping(iCurrentLocation,ECurrentLocation);
	
	iCurrentTemperature = new WeatherGraphicsWidget("",KCurrentTemperatureFont);
	iCurrentTemperature->setToolTip("Click to refresh");
	iCurrentTemperature->setAlignment(Qt::AlignRight);
	connect(iCurrentTemperature, SIGNAL(itemExecuted()), iSignalMapper, SLOT(map()));
	iSignalMapper->setMapping(iCurrentTemperature,ECurrentTemperature);
	
	iCurrentTemperatureUnits = new WeatherGraphicsWidget("",KTemperatureUnitsFont,Qt::AlignHCenter);
	iCurrentTemperatureUnits->setToolTip("Click to swap units");
	connect(iCurrentTemperatureUnits, SIGNAL(itemExecuted()), iSignalMapper, SLOT(map()));
	iSignalMapper->setMapping(iCurrentTemperatureUnits,ECurrentTemperatureUnits);
	
	iCurrentTemperatureHighLow = new WeatherGraphicsWidget("",KCurrentLocationLevel3Font,Qt::AlignRight);
	iCurrentCondition = new WeatherGraphicsWidget("Connecting...",KCurrentConditionFont);
	connect(iCurrentCondition, SIGNAL(itemExecuted()), iSignalMapper, SLOT(map()));
	iSignalMapper->setMapping(iCurrentCondition,ECurrentCondition);

	// create buttons
	iCloseButton = new GraphicsWidgetItem(new QGraphicsPixmapItem(QPixmap(":/resources/images/buttons/close.png")));
	iCloseButton->setToolTip("Close");
	connect(iCloseButton, SIGNAL(clicked()), iSignalMapper, SLOT(map()));
	iSignalMapper->setMapping(iCloseButton,EClose);

	iMinimizeButton =  new GraphicsWidgetItem(new QGraphicsPixmapItem(QPixmap(":/resources/images/buttons/minimize.png")));
	iMinimizeButton->setToolTip("Minimize");
	connect(iMinimizeButton, SIGNAL(clicked()), iSignalMapper, SLOT(map()));
	iSignalMapper->setMapping(iMinimizeButton,EMinimize);
	
	iSettingsButton = new GraphicsWidgetItem(new QGraphicsPixmapItem(QPixmap(":/resources/images/buttons/settings.png")));
	iSettingsButton->setToolTip("Settings");
	connect(iSettingsButton, SIGNAL(clicked()), iSignalMapper, SLOT(map()));
	iSignalMapper->setMapping(iSettingsButton,ESettings);
	
	iHelpButton = new GraphicsWidgetItem(new QGraphicsPixmapItem(QPixmap(":/resources/images/buttons/help.png")));
	iHelpButton->setToolTip("Help");
	connect(iHelpButton, SIGNAL(clicked()), iSignalMapper, SLOT(map()));
	iSignalMapper->setMapping(iHelpButton,EHelp);

	iFavoritesButton = new GraphicsWidgetItem(new QGraphicsPixmapItem(QPixmap(":/resources/images/buttons/favorites.png")));
	iFavoritesButton->setToolTip("Favorites");
	connect(iFavoritesButton, SIGNAL(clicked()), iSignalMapper, SLOT(map()));
	iSignalMapper->setMapping(iFavoritesButton,EFavorites);
	
	iMapsButton = new GraphicsWidgetItem(new QGraphicsPixmapItem(QPixmap(":/resources/images/buttons/maps.png")));
	iMapsButton->setToolTip("Show on maps");
	connect(iMapsButton, SIGNAL(clicked()), iSignalMapper, SLOT(map()));
	iSignalMapper->setMapping(iMapsButton,EMaps);
	
	//
	// By this step all widgets are created, add them to layouts
	addItemsToLayout();
	
qDebug() << "WeatherGraphicsWindow::createAndAddWeatherItems<-";	
}

void WeatherGraphicsWindow::addItemsToLayout()
{
	const int verticalspacing = 50;
	const int extraheight = 40;
	int accumulatedHeight = 0;
	qreal width  = (KWindowWidth)/2;

	QGraphicsAnchor* anchor;
	iAnchorLayout = new QGraphicsAnchorLayout;
	setLayout(iAnchorLayout);
	accumulatedHeight += iAnchorLayout->verticalSpacing();
	
	// Vertical temperature layout to hold current temp, high/low
	iVerticalCurrentTempLayout = new QGraphicsLinearLayout(Qt::Vertical);
	iVerticalCurrentTempLayout->setPreferredSize(((80*width)/100),
													iCurrentTemperature->getTextHeight()+extraheight);
	accumulatedHeight += iVerticalCurrentTempLayout->preferredHeight();
	
	iVerticalCurrentTempLayout->addItem(iCurrentTemperature);
	iCurrentTemperatureHighLow->setMaximumHeight((20*(iCurrentTemperature->getTextHeight()+extraheight))/100);
	iVerticalCurrentTempLayout->addItem(iCurrentTemperatureHighLow);
	iVerticalCurrentTempLayout->setStretchFactor(iCurrentTemperature,2);
	iCurrentTemperatureUnits->setPreferredSize(((20*width)/100),
									 			iCurrentTemperatureUnits->getTextHeight()+extraheight);

	// anchor to top of layout
	anchor = iAnchorLayout->addAnchor(iVerticalCurrentTempLayout,Qt::AnchorTop,
							 		  iAnchorLayout,Qt::AnchorTop);
	anchor->setSpacing(verticalspacing);
	accumulatedHeight += verticalspacing;
	anchor = iAnchorLayout->addAnchor(iVerticalCurrentTempLayout,Qt::AnchorLeft,
								 	  iAnchorLayout,Qt::AnchorLeft);
	
	// set temperature units next to temperature
	anchor = iAnchorLayout->addAnchor(iVerticalCurrentTempLayout,Qt::AnchorRight,
									  iCurrentTemperatureUnits,Qt::AnchorLeft);
	anchor->setSpacing(0);
	anchor = iAnchorLayout->addAnchor(iVerticalCurrentTempLayout,Qt::AnchorTop,
										  iCurrentTemperatureUnits,Qt::AnchorTop);
	
	//match sizes
	iAnchorLayout->addAnchors(iVerticalCurrentTempLayout,iCurrentTemperatureUnits,Qt::Vertical);
	
	// location
	//
	// this layout holds city,country,laction local time
	iVerticalLocationLayout = new QGraphicsLinearLayout(Qt::Vertical);
	iVerticalLocationLayout->setPreferredSize(width,iCurrentTemperature->getTextHeight()+extraheight);
	iVerticalLocationLayout->setSpacing(0);
	
	// add location items to location layout
	iVerticalLocationLayout->addItem(iCurrentLocation);
	iVerticalLocationLayout->addItem(iCurrentLocationLevel2);
	iVerticalLocationLayout->addItem(iCurrentLocationLevel3);
	
	// add anchors to location layout
	anchor = iAnchorLayout->addAnchor(iVerticalLocationLayout,Qt::AnchorTop,
									  iAnchorLayout,Qt::AnchorTop);
	anchor->setSpacing(verticalspacing);
	anchor = iAnchorLayout->addAnchor(iVerticalLocationLayout,Qt::AnchorLeft,
										  iCurrentTemperatureUnits,Qt::AnchorRight);
	anchor->setSpacing(30);
	anchor = iAnchorLayout->addAnchor(iVerticalLocationLayout,Qt::AnchorRight,
											  iAnchorLayout,Qt::AnchorRight);
	
	// line break
	GraphicsWidgetItem *lineitem = new GraphicsWidgetItem(new QGraphicsLineItem(QLine(0,0,KWindowWidth-20,0)));
	
	iAnchorLayout->addCornerAnchors(lineitem,Qt::TopLeftCorner,
			iVerticalCurrentTempLayout,Qt::BottomLeftCorner);
	anchor = iAnchorLayout->addAnchor(lineitem,Qt::AnchorLeft,
									  iAnchorLayout,Qt::AnchorLeft);
	anchor->setSpacing(0);
	anchor = iAnchorLayout->addAnchor(lineitem,Qt::AnchorRight,
										  iAnchorLayout,Qt::AnchorRight);
	anchor->setSpacing(0);
	accumulatedHeight += lineitem->preferredHeight();
	
	// Current Condition
	iAnchorLayout->addCornerAnchors(iCurrentCondition,Qt::TopLeftCorner,
			iVerticalCurrentTempLayout,Qt::BottomLeftCorner);
	anchor = iAnchorLayout->addAnchor(iCurrentCondition,Qt::AnchorRight,
									  iAnchorLayout,Qt::AnchorRight);
	
	// Forecast
	iHorizontalForecastLayout = new QGraphicsLinearLayout(Qt::Horizontal);
	iHorizontalForecastLayout->setPreferredSize(KWindowWidth,KWindowWidth/KMaxForecastItems);
	iHorizontalForecastLayout->setSpacing(0);
	
	// Create forecast items
	for(int i=0;i<KMaxForecastItems;i++)
	{
		iForecastWidgetsList.append(new WeatherForecastWidget);
		iForecastWidgetsList[i]->setToolTip("Click for detailed forecast");
		iForecastWidgetsList[i]->setPreferredSize(KWindowWidth/KMaxForecastItems,KWindowWidth/KMaxForecastItems);
		iHorizontalForecastLayout->addItem(iForecastWidgetsList[i]);
		connect(iForecastWidgetsList[i],SIGNAL(displayDetailedForecast(bool,const QStringList&)),
				iForecastDialog,SLOT(displayTextWidget(bool,const QStringList&)));
	}

	anchor = iAnchorLayout->addAnchor(iHorizontalForecastLayout,Qt::AnchorTop,
								 	  iAnchorLayout,Qt::AnchorTop);
	iAnchorLayout->updateGeometry();
	anchor->setSpacing(accumulatedHeight+15); // adding height for good look
	anchor = iAnchorLayout->addAnchor(iHorizontalForecastLayout,Qt::AnchorLeft,
									 	  iAnchorLayout,Qt::AnchorLeft);
	
	anchor = iAnchorLayout->addAnchor(iHorizontalForecastLayout,Qt::AnchorRight,
										 	  iAnchorLayout,Qt::AnchorRight);
	
	// botton ribbon
	iHorizontalRibbonLayout = new QGraphicsLinearLayout(Qt::Horizontal);
	iHorizontalRibbonLayout->setPreferredSize(128,32); //plan to use 32x32 images
	
	// add items to layout
	iHorizontalRibbonLayout->setSpacing(0);
	iHorizontalRibbonLayout->addItem(iCloseButton);
	iHorizontalRibbonLayout->addItem(iMinimizeButton);
	iHorizontalRibbonLayout->addItem(iSettingsButton);
	iHorizontalRibbonLayout->addItem(iFavoritesButton);
	iHorizontalRibbonLayout->addItem(iMapsButton);
	iHorizontalRibbonLayout->addItem(iHelpButton);

	anchor = iAnchorLayout->addAnchor(iHorizontalRibbonLayout,Qt::AnchorRight,
									  iAnchorLayout,Qt::AnchorRight);
	anchor = iAnchorLayout->addAnchor(iHorizontalRibbonLayout,Qt::AnchorLeft,
									  iAnchorLayout,Qt::AnchorLeft);
	anchor = iAnchorLayout->addAnchor(iHorizontalRibbonLayout,Qt::AnchorBottom,
										  iAnchorLayout,Qt::AnchorBottom);
	anchor->setSpacing(0);
}

// update weather items to reflect change 
void WeatherGraphicsWindow::updateWeatherItems(bool updateOnlyTemperatureItems)
{
	// TODO: check and update only if values are different
	
	iCurrentTemperature->setText(iCurrentWeatherInfo.iCurrentTemperature);
	iCurrentTemperatureUnits->setText(iCurrentWeatherInfo.iUnits);
	iCurrentTemperatureHighLow->setText(tr("Low:") + iCurrentWeatherInfo.iLow+"  "+tr("High:")+iCurrentWeatherInfo.iHigh);
	emit temperatureUpdated(iCurrentWeatherInfo.iCurrentTemperature+iCurrentWeatherInfo.iUnits);
	
	if(!updateOnlyTemperatureItems)
	{
	iCurrentLocation->setText(iCurrentLocationInfo.iCityName );
	iCurrentLocationLevel2->setText(iCurrentLocationInfo.iCountryName);
	iCurrentCondition->setText(iCurrentWeatherInfo.iCurrentCondition);
	iCurrentLocationLevel3->setText("DateTime");
	
	// Get mapped pixmaps for the current condition image
	iMappedPixmapList.clear();
	getMappedPixmaps(iCurrentWeatherInfo.iCurrentConditionImage,
					 iMappedPixmapList,
					 iCurrentWeatherInfo.iMoonPhaseImage,
					 true);
	}
	this->setVisible(true);
}

void WeatherGraphicsWindow::updateConditionImages(bool display)
{
	if(display)
	{
		// Create new pixmap items 
		// Get mapped pixmaps for the current condition image
		/*QList<QPixmap> mappedPixmapList;
		getMappedPixmaps(iCurrentWeatherInfo.iCurrentConditionImage,
						 mappedPixmapList,
						 iCurrentWeatherInfo.iMoonPhaseImage,
						 true);*/

		// update pixmapitems
		if(!iMappedPixmapList.isEmpty())
		{
			iGraphicsPixmapItemList.clear();
			for(int i=0;i<iMappedPixmapList.count();i++)
			{
				// append new item to scene
				iGraphicsPixmapItemList.append(this->scene()->addPixmap(iMappedPixmapList[i]));
				iGraphicsPixmapItemList[i]->setOpacity(0);
				iGraphicsPixmapItemList[i]->setTransformationMode(Qt::SmoothTransformation);
				iGraphicsPixmapItemList[i]->setZValue(0);
				int effectiveImageOutside = 54; // % of image outside 
				
				// special handling for base layers images
				if(i>0)
				{
					//iGraphicsPixmapItemList[i]->setOpacity(0.7);
					effectiveImageOutside = 30;
				}
				
				// move to required position
				int pixmapX = (KWindowWidth - iMappedPixmapList[i].width())/2;
				int pixmapY = ((effectiveImageOutside*iMappedPixmapList[i].height())/100); // % of image outside 
				iGraphicsPixmapItemList[i]->setPos(QPointF(pixmapX,-pixmapY)); // -ve value indicated that we want to display it out of bounding rect
			}

		// Display pixmaps with animation	
		QPropertyAnimation* fadeout = new QPropertyAnimation(this,"conditionImageOpacity");
		fadeout->setDuration(2000);
		fadeout->setStartValue(qreal(0));
		fadeout->setEndValue(qreal(1));
		fadeout->setEasingCurve(QEasingCurve::Linear);
		fadeout->start(QAbstractAnimation::DeleteWhenStopped);
		}	
	}
	
	else
	{
		QPropertyAnimation* fadein = new QPropertyAnimation(this,"conditionImageOpacity");
		fadein->setDuration(1000);
		fadein->setStartValue(qreal(1));
		fadein->setEndValue(qreal(0));
		fadein->setEasingCurve(QEasingCurve::Linear);
		fadein->start(QAbstractAnimation::DeleteWhenStopped);
		
		// connect finished signal of animation so that we can delete the pixmaps in that slot
		QObject::connect(fadein, SIGNAL(finished()), this, SLOT(removeConditionImages()));
	}
}


void WeatherGraphicsWindow::removeConditionImages()
{
	// remove previous items from scene
	for(int i=0;i<iGraphicsPixmapItemList.count();i++)
	{
		iGraphicsPixmapItemList[i]->setVisible(false);
		this->scene()->removeItem(iGraphicsPixmapItemList[i]);
	}
	iGraphicsPixmapItemList.clear();
}

void WeatherGraphicsWindow::loadGoogleMaps(const QString aLat,
										   const QString aLong,
										   const QString aText)
{
	QString request = KGoogleMapsRequest.arg(aLat)
										.arg(aLong)
										.arg(aText);
	qDebug()<<"WeatherGraphicsWindow::loadGoogleMaps:"<<request;
	QUrl urltoload(request);
	QDesktopServices::openUrl(urltoload);
}

void WeatherGraphicsWindow::getWeatherUpdate(const LocationInfoT& aLocation,
										     const RequestMethodT& aRequestType)
{
qDebug()<<"WeatherGraphicsWindow::getWeatherInfo->";
	
// Decide which units to request
bool requestInC = true;
if(KFahrenheit == iCurrentWeatherInfo.iUnits)
{
	requestInC = false;
}

	// assumes either of the station codes are filled
	if(aLocation.iCityCode.length())
	{
		iNetworkEngine->getWeatherInfo(aLocation.iCityCode,ECitycode,requestInC,aRequestType);
	}
	
	else /*if(aLocation.iZipCode.length())*/
	{
		iNetworkEngine->getWeatherInfo(aLocation.iZipCode,EZipcode,requestInC,aRequestType);
	}
	// network response will be handled in the respective slot

qDebug()<<"WeatherGraphicsWindow::getWeatherInfo<-";	
}

void WeatherGraphicsWindow::on_signalmapper_mapped(int aItemId)
{
qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped->";
	switch(aItemId)
	{
		case ECurrentLocation: 
		{
			qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped ECurrentLocation";
			showLocationDialog(iLocationList);
		}
		break;	
			
		case ECurrentTemperature:
		{
			qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped ECurrentTemperature";
			getWeatherUpdate(iCurrentLocationInfo,ELiveRSS);
		}
		break;
		case ECurrentTemperatureUnits:
		{
			qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped ECurrentTemperatureUnits";
			swapTemperatureUnits();
		}
		break;
		case ECurrentCondition:
		{
			qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped ECurrentCondition";
			// TODO: show extended info
		}
		break;
		
		case EClose:
		{
			qApp->exit();
		}
		break;
		
		case EMinimize:
		{
			//TODO: add minimize to tray code
			emit minimize();
		}
		break;

		case ESettings:
		{
			iStatusDialog->displayTextWidget(!iStatusDialog->isVisible(),QStringList("Settings"));
		}
		break;
		
		case EFavorites:
		{
			iStatusDialog->displayTextWidget(!iStatusDialog->isVisible(),QStringList("Favorites"));
		}
		break;
		
		case EMaps:
		{
			loadGoogleMaps(iCurrentWeatherInfo.iLatitude,iCurrentWeatherInfo.iLongitude,iCurrentLocationInfo.iCityName);			
		}
		break;
		
		case EHelp:
		{
			QStringList stringlist;
			stringlist.append("WeatherWidget2 - Help");
			stringlist.append("Click on city name to change location.<BR>Click on temperture to update.<BR>Click on units to swap from C to F or viceversa.<BR>Click on forecast items for detailed forecast.");
			iStatusDialog->displayTextWidget(!iStatusDialog->isVisible(),stringlist);
		}
		break;

		default:
		{
			qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped default";
		}
		break;
	}
qDebug()<<"WeatherGraphicsWindow::on_signalmapper_mapped<-";	
}

void WeatherGraphicsWindow::on_networkengine_weatherRequestCompleted(const WeatherResponseT& aResponse,
																	 RequestMethodT aRequestMethod,
						 				  			   			     bool aError)
{
qDebug()<<"WeatherGraphicsWindow::on_networkEngine_weatherRequestCompleted->";
qDebug()<<"<RESPONSE START>";
qDebug()<<aResponse.iResponseBuffer;
qDebug()<<aResponse.iStatusDescription;
qDebug()<<aResponse.iStatusCode;
qDebug()<<"<RESPONSE END>";

setStatusText(KProductName,false);

	// Some network error
	if(aError)
	{
		qDebug()<<"Error: "<<aError;
		QStringList dlglist;
		dlglist.append("Opps!");
		dlglist.append("Network problem.<BR>"+aResponse.iStatusDescription);
		iStatusDialog->displayTextWidget(true,dlglist);
		iCurrentCondition->setText("Trying to connect...");
		getWeatherUpdate(iCurrentLocationInfo);
		return;
	}
	
	if(ESearchLocationFromString == aRequestMethod)
	{
		iLocationList.clear();
		bool stat = WeatherWidgetXMLEngine::getLocationDetails(aResponse.iResponseBuffer,iLocationList);
		
		// TODO: comeup with a timed error note
		// parsing failed
		if(false == stat)
		{
			QStringList dlglist;
			dlglist.append("Opps!");
			dlglist.append("No locations found.");
			iStatusDialog->displayTextWidget(true,dlglist);
			return;
		}
		
		// multiple locations are returned
		if(iLocationList.count())
		{
			// let user select
			showLocationDialog(iLocationList);
		}
		
		// single location
		else if(1 == iLocationList.count())
		{
		iCurrentLocationInfo = iLocationList[0];
		iCurrentLocation->setText(iCurrentLocationInfo.iCityName);
		iCurrentLocationLevel2->setText(iCurrentLocationInfo.iCountryName);
		//update();
		getWeatherUpdate(iCurrentLocationInfo,ELiveRSS);
		}
	}
	
	else if(ELiveRSS == aRequestMethod)
	{
		bool stat = WeatherWidgetXMLEngine::getWeatherInfo(aResponse.iResponseBuffer,iCurrentWeatherInfo);
		if(false == stat)
		{
			QStringList dlglist;
			dlglist.append("Opps!");
			dlglist.append("Cannot get weather update.");
			iStatusDialog->displayTextWidget(true,dlglist);
			return;
		}
		
		qDebug()<<"Recieved weather data \ntemp: "<<iCurrentWeatherInfo.iCurrentTemperature<<" condition: "<<iCurrentWeatherInfo.iCurrentCondition;
		
		// remove period symbol, we need temperature without decimal value
		int index = iCurrentWeatherInfo.iCurrentTemperature.indexOf(QChar('.'));
		if(index >=0)
		{
			iCurrentWeatherInfo.iCurrentTemperature = iCurrentWeatherInfo.iCurrentTemperature.left(index);
		}
		iCurrentWeatherInfo.iCurrentTemperature.append(QChar(KDegSymbol));

		// get local time
		iNetworkEngine->getLocalTime(iCurrentWeatherInfo.iLatitude,iCurrentWeatherInfo.iLongitude);
	}
	
	else if(EForecast == aRequestMethod)
	{
		qDebug()<<"EForecast request completed with HTTP status code "<<aResponse.iStatusCode<<" "<<aResponse.iStatusDescription;
		WeatherWidgetXMLEngine::getForecastInfo(aResponse.iResponseBuffer,iForecastList);
		
		if(iForecastList.count())
		{
			for(int i=0;i<KMaxForecastItems;i++)
			{
				iForecastList[i].iHigh.append(QChar(KDegSymbol));
				iForecastList[i].iLow.append(QChar(KDegSymbol));
				
				// get mapped pixmaps
				getMappedPixmaps(iForecastList[i].iIcon,iForecastList[i].iConditionPixmapList);
				
				// set this info to forecast widget
				iForecastWidgetsList[i]->setForecastInfo(iForecastList[i]);
			}
		}
		
		// no forecast OR network error
		else
		{
			QStringList dlglist;
			dlglist.append("Opps!");
			dlglist.append("Cannot get forecast.");
			iStatusDialog->displayTextWidget(true,dlglist);
			return;

		}
		//updateWeatherItems(this->iCurrentLocationInfo,this->iCurrentWeatherInfo);
		animateAngle(0,360);
	}
	
	// local time
	else if(ELocalTime == aRequestMethod)
	{
	bool found = WeatherWidgetXMLEngine::getLocalTime(aResponse.iResponseBuffer,iLocalDateTime);

		// kill timer for datetime
		killTimer(iTimerIdLocalTimeupdate);
		
		// found valid datetime
		if(found && iLocalDateTime.isValid())
		{
			// update display
			iCurrentLocationLevel3->setText(iLocalDateTime.toString(Qt::SystemLocaleShortDate));
			iTimerIdLocalTimeupdate = startTimer(KLocalTimeUpdateIntervalInMs);	
		}
		
		else
		{
			// clear local time display
			iCurrentLocationLevel3->setText("");	
		}

	getWeatherUpdate(iCurrentLocationInfo,EForecast);
	}
qDebug()<<"WeatherGraphicsWindow::on_networkEngine_weatherRequestCompleted<-";	
}

void WeatherGraphicsWindow::on_networkengine_weatherRequestStarted()
{
	// Update Status Text
	setStatusText(tr("Loading..."));
}

void WeatherGraphicsWindow::showLocationDialog(const QList<LocationInfoT>& aLocationList,bool aSavedList)
{
qDebug()<<"WeatherGraphicsWindow::showLocationDialog->";

	// User need to select from multiple locations resulted from searchkey
	if(aLocationList.count())
	{
		// populate list	
		QStringList locations;
		for(int i=0;i<aLocationList.count();i++)
		{
			locations.append(aLocationList[i].iCityName+","+aLocationList[i].iCountryName+ tr(" Pincode:")+aLocationList[i].iZipCode);
			qDebug()<<locations[i];
		}
		
		iSelectLocation->ui.listWidget->clear();
		iSelectLocation->ui.listWidget->insertItems(0,locations);
		iSelectLocation->ui.listWidget->setVisible(true);
		//TODO: set list label visible
	}

	// showing location dialog for user to input searchkey
/*	else
	{
		iSelectLocation->ui.listWidget->setVisible(false);
	}*/

	// TODO: do resize, cosmetic changes here
	
	// show selection dialog
	iSelectLocation->show();
	
	
qDebug()<<"WeatherGraphicsWindow::showLocationDialog<-";	
}

void WeatherGraphicsWindow::setStatusText(const QString& aText,bool display)
{
	// update status text
	//qgraphicsitem_cast<QGraphicsTextItem*>(iStatusText->graphicsItem())->setPlainText(aText);
	if(display)
	{
	iCurrentCondition->setText(aText);
	}
	
	else
	{
	iCurrentCondition->setText("");	
	}
}

void WeatherGraphicsWindow::selectlocation_okclicked()
{
qDebug()<<"WeatherGraphicsWindow::selectlocation_okclicked->";
iSelectLocation->close();

	// user selected from list
	if(iSelectLocation->ui.listWidget->count() && 
	   iSelectLocation->ui.listWidget->currentRow() >= 0)
	{
		// get selected location 
		iCurrentLocationInfo = iLocationList[iSelectLocation->ui.listWidget->currentRow()];
		iUserSelectedLocationList.append(iCurrentLocationInfo);
		iSelectLocation->ui.listWidget->clear();
		
		// get weather update for the location
		iMainWindowAnimationRequired = true;
		getWeatherUpdate(iCurrentLocationInfo,ELiveRSS);
	}
	
	else
	{
		// fetch location info for searchkey
		iNetworkEngine->getStationCode(iSelectLocation->ui.lineEdit->text());
	}
	
qDebug()<<"WeatherGraphicsWindow::selectlocation_okclicked<-";
}

void WeatherGraphicsWindow::paint(QPainter *painter, 
								  const QStyleOptionGraphicsItem *option,
           						  QWidget *widget)
{
#define FILL_WITH_COLOR	
	// draw to this painter path
	QPainterPath rounded_rect;
	rounded_rect.addRoundRect(rect(),10);

#ifdef FILL_WITH_COLOR	
	// fill background with color
	painter->setOpacity(0.75);
	
/*	QLinearGradient gradient(0, 0, 0, this->rect().height());
	gradient.setSpread(QGradient::ReflectSpread);
	gradient.setColorAt(0.0, Qt::black);
	gradient.setColorAt(0.4, Qt::gray);
	gradient.setColorAt(0.6, Qt::black);
	gradient.setColorAt(1.0, Qt::black);
	painter->fillPath(rounded_rect,QBrush(gradient));*/
	painter->fillPath(rounded_rect,QBrush(Qt::black));
	painter->setOpacity(1);
	
	//glass highlight
/*	painter->setBrush(QBrush(Qt::white));
	painter->setPen(QPen(QBrush(Qt::white), 0.01));
	painter->setOpacity(0.30);
	painter->drawPath(rounded_rect);//drawRect(1, 1, rect().width() - 2, (rect().height() / 2) - 2);*/
#else
	painter->fillPath(rounded_rect,QBrush(QPixmap(KBackgroundImagesRoot+"brushedmetal.jpg")));
#endif
}

void WeatherGraphicsWindow::timerEvent(QTimerEvent* aEvent)
{
	// update weather info
	if(aEvent->timerId() == iTimerIdWeatherupdate)
	{
		getWeatherUpdate(iCurrentLocationInfo,ELiveRSS);
	}
	
	// local time update
	else if(aEvent->timerId() == iTimerIdLocalTimeupdate)
	{
		iLocalDateTime = iLocalDateTime.addMSecs(KLocalTimeUpdateIntervalInMs);
		iCurrentLocationLevel3->setText(iLocalDateTime.toString(Qt::SystemLocaleShortDate));
		iCurrentLocationLevel3->update();
	}
}

void WeatherGraphicsWindow::getMappedPixmaps(const QString& aCurrentConditionImageId,
											 QList<QPixmap>& aPixmapList,
											 const QString& aMoonphaseimageId,
											 bool aLoadMoonPhase)
{
	// try getting mapped image
	if(!aCurrentConditionImageId.isEmpty())
	{
	bool isday = true;

	// Check if it is day or night
	if( aLoadMoonPhase && 
	    (iLocalDateTime.time().hour() >= 18 || iLocalDateTime.time().hour() <= 6))
	{
		// its night
		isday = false;
	}

	QFile mappedImagesXML(KImageMappingXMLPath);
		if(mappedImagesXML.open(QIODevice::ReadOnly))
		{
		qDebug()<<"WBImageid :"<<aCurrentConditionImageId.rightRef(3);
		
		ConditionImageDetailsT mappedImageDetails;
		mappedImageDetails.iImagesList.clear();
		// found a mapped image	
		if(WeatherWidgetXMLEngine::getMappedCondtionImageDetails(aCurrentConditionImageId.rightRef(3),
																 mappedImagesXML.readAll(),
																 mappedImageDetails))
		
		{
				qDebug()<<"Image mapped";
				qDebug()<<"layers    :"<<mappedImageDetails.iLayers;
				qDebug()<<"isdayimage:"<<mappedImageDetails.iDayImage;
				
				for(int i=0;i<mappedImageDetails.iLayers;i++)
				{
					// night, so load moonphase instead dayimage
					if(0==i && !isday && aLoadMoonPhase)
					{
						qDebug()<<"moonphase image id:"<<aMoonphaseimageId;
						if(aLoadMoonPhase && !aMoonphaseimageId.isEmpty())
						{
						aPixmapList.append(QPixmap(KMoonphasesImagesRoot+aMoonphaseimageId+".png"));
						qDebug()<<"appending moonphase image:"<<aMoonphaseimageId;
						}
						
						// skip loading dayimage
						if(mappedImageDetails.iDayImage){continue;}
					}
					qDebug()<<"appending image:"<<mappedImageDetails.iImagesList[i];
					aPixmapList.append(KMappedImagesRoot+mappedImageDetails.iImagesList[i]);
				}
		} // found mapped images
		
		} // opened mappedimages.xml
		
	} // valid condition image id
}


void WeatherGraphicsWindow::setRotationAngleY(qreal angle)
{
	setVisible(true);
    QPointF c = boundingRect().center();
    QTransform t;
    t.translate(c.x(), c.y());
    t.rotate(angle, Qt::YAxis);
    c = boundingRect().center();
    t.translate(-c.x(), -c.y());
    setTransform(t);
}

void WeatherGraphicsWindow::setConditionImageOpacity(qreal opacity)
{
	iGraphicsPixmapItemList.count();
	qDebug()<<"WeatherGraphicsWindow::setConditionImageOpacity recieved opacity:"<<opacity;
	for(int i=0;i<iGraphicsPixmapItemList.count();i++)
	{
		/*if(opacity < 0.5)
		{
			iGraphicsPixmapItemList[i]->setVisible(false);
		}
		else*/
		{
		//iGraphicsPixmapItemList[i]->setVisible(true);
		iGraphicsPixmapItemList[i]->setOpacity(opacity);
		}
	}
}

void WeatherGraphicsWindow::animateAngle(int startAngle, int endAngle)
{
	// remove condition images
	updateConditionImages(false);
	
    int animtime(1000);
    // Create rotation animation
    QPropertyAnimation* rotanim = new QPropertyAnimation(this, "rotationAngleY");
    rotanim->setDuration(animtime/2);
    rotanim->setStartValue(startAngle);
    rotanim->setEndValue(endAngle/2);
    rotanim->setEasingCurve(QEasingCurve::Linear);
    
    // Connect to finished signal so that we can load the new weather info
    QObject::connect(rotanim, SIGNAL(finished()), this, SLOT(updateWeatherItems()));
    
    // Create rotation animation
    QPropertyAnimation* rotanim2 = new QPropertyAnimation(this, "rotationAngleY");
    rotanim2->setDuration(animtime/2);
    rotanim2->setStartValue(endAngle/2);
    rotanim2->setEndValue(endAngle);
    rotanim2->setEasingCurve(QEasingCurve::Linear);

    // Connect to finished signal so that we can start icon animation 
    QObject::connect(rotanim2, SIGNAL(finished()), this, SLOT(updateConditionImages()));
    
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup(this);
	group->addAnimation(rotanim);
	group->addAnimation(rotanim2);
	group->start(QAbstractAnimation::DeleteWhenStopped);
}


void WeatherGraphicsWindow::swapTemperatureUnits()
{
bool convertToC = false;

	// Convert from F to C
	if(KFahrenheit == iCurrentWeatherInfo.iUnits)
	{
		convertToC = true;
		iCurrentWeatherInfo.iUnits = KCelcius;
	}

	// Convert from C to F
	else
	{
		iCurrentWeatherInfo.iUnits = KFahrenheit;
	}

	getWeatherUpdate(iCurrentLocationInfo,ELiveRSS);
	return;
	
		// First convert main temperature
		int tmpToConvert = iCurrentWeatherInfo.iCurrentTemperature.left(iCurrentWeatherInfo.iCurrentTemperature.length()-1).toInt();
		int converted = convertTemperature(tmpToConvert,convertToC); 
		iCurrentWeatherInfo.iCurrentTemperature = QString("%1%2").arg(converted)  
												                 .arg(QChar(KDegSymbol));
		
		// Convert high value
		tmpToConvert = iCurrentWeatherInfo.iHigh.left(iCurrentWeatherInfo.iHigh.length()-1).toInt();
		converted = convertTemperature(tmpToConvert,convertToC); 
		iCurrentWeatherInfo.iHigh = QString("%1%2").arg(converted)  
												   .arg(QChar(KDegSymbol));
		
		// Convert low value
		tmpToConvert = iCurrentWeatherInfo.iLow.left(iCurrentWeatherInfo.iLow.length()-1).toInt();
		converted = convertTemperature(tmpToConvert,convertToC); 
		iCurrentWeatherInfo.iLow = QString("%1%2").arg(converted)  
										          .arg(QChar(KDegSymbol));
		
		// update only temperature items
		updateWeatherItems(true);

		// Convert forecast items temperature component
		for(int i=0;i<iForecastWidgetsList.count();i++)
		{
			tmpToConvert = iForecastList[i].iHigh.left(iForecastList[i].iHigh.length()-1).toInt();
			converted = convertTemperature(tmpToConvert,convertToC); 
			iForecastList[i].iHigh = QString("%1%2").arg(converted)  
												    .arg(QChar(KDegSymbol));
			
			tmpToConvert = iForecastList[i].iLow.left(iForecastList[i].iLow.length()-1).toInt();
			converted = convertTemperature(tmpToConvert,convertToC); 
			iForecastList[i].iLow = QString("%1%2").arg(converted)  
											       .arg(QChar(KDegSymbol));

			// update forecast items with converted temperatures
			iForecastWidgetsList[i]->updateTemperature(iForecastList[i]);
			iForecastWidgetsList[i]->update();
		}
}
	

int WeatherGraphicsWindow::convertTemperature(int toConvert, bool convertToCelcius)
{
	int convertedvalue;
	// Convert from F to C
	if(convertToCelcius)
	{
		convertedvalue = ((qreal(5)/qreal(9))*(toConvert-32));
	}
	
	// Convert from C to F
	else
	{
		convertedvalue =  ((qreal(9)/qreal(5))*(toConvert+32));
	}
	
	return convertedvalue;
}




// eof
