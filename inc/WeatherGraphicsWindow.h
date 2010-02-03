
// This is highlevel window for weather info

#ifndef WEATHERGRAPHICSWINDOW_H_
#define WEATHERGRAPHICSWINDOW_H_

#include <QGraphicsWidget>
#include <QSignalMapper>
#include <QGraphicsTextItem>


#include "WeatherGraphicsWidget.h"
#include "WeatherWidgetNetworkEngine.h"
#include "WeatherWidgetXMLEngine.h"
#include "GraphicsWidgetItem.h"
#include "WeatherDialog.h"

enum TemeratureConversion
{
	EFahrenheit,
	ECelsius 
};
// This is a widget to hold the layout. 
//
class SelectLocation;
class WeatherGraphicsRibbon;
class QGraphicsAnchorLayout;
class WeatherForecastWidget;
class WeatherGraphicsWindow:public QGraphicsWidget
{
	Q_OBJECT
	// For property animation
    Q_PROPERTY(qreal rotationAngleY READ rotationAngleY WRITE setRotationAngleY)
	
	// TODO: We should not do like this :(
	Q_PROPERTY(qreal conditionImageOpacity READ conditionImageOpacity WRITE setConditionImageOpacity)

public:
	WeatherGraphicsWindow(QObject* aParent=0);
	~WeatherGraphicsWindow();
	
public:
	void paint(QPainter *painter, 
			   const QStyleOptionGraphicsItem *option,
	           QWidget *widget = 0);

	void timerEvent(QTimerEvent* aEvent);

// Properties	
public:
    // For rotationAngleY property
    void setRotationAngleY(qreal angle);
	qreal rotationAngleY() const{return 0;}
	
	void setConditionImageOpacity(qreal opacity);
	qreal conditionImageOpacity() const{return 0;}
private:
	void createAndAddWeatherItems();

	void addItemsToLayout();
	
	void getWeatherUpdate(const LocationInfoT& aLocation,
						const RequestMethodT& aRequestType = ELiveRSS);
	
	void showLocationDialog(const QList<LocationInfoT>& aLocationList,bool aSavedList=false);
	
	void setStatusText(const QString& aText,bool display = true);
	
	void getMappedPixmaps(const QString& aCurrentConditionImageId,
						  QList<QPixmap>& aPixmapList,
						  const QString& aMoonphaseimageId = QString(),
						  bool aLoadMoonPhase = false);
protected:
	void animateAngle(int startAngle, int endAngle);
	void swapTemperatureUnits();
	int convertTemperature(int toConvert, bool convertToCelcius);
private	slots:
	// QSignalMapper
	void on_signalmapper_mapped(int);

	// WeatherWidgetNetworkEngine
	void on_networkengine_weatherRequestCompleted(const WeatherResponseT& aResponse,
												  RequestMethodT aRequestMethod,
											      bool aError);
	void on_networkengine_weatherRequestStarted();
	
	void selectlocation_okclicked();
	
	void updateWeatherItems(bool updateOnlyTemperatureItems = false);
	void updateConditionImages(bool display=true);
	void removeConditionImages();
	void loadGoogleMaps(const QString aLat,
						const QString aLong,
						const QString aText);
	
signals:
	void minimize();
	void temperatureUpdated(const QString&);
private:
	
	// Flags
	bool iMainWindowAnimationRequired;
	int iUpdateIntervalInMs;
	
	// Timer ids
	int iTimerIdWeatherupdate;
	int iTimerIdLocalTimeupdate;
	WeatherWidgetNetworkEngine* iNetworkEngine;

	// datetime of current location
	QDateTime iLocalDateTime; 
	QSignalMapper* iSignalMapper;

	// Layouts
	//	
	// Container layout for current temperature
	QGraphicsLinearLayout* iVerticalCurrentTempLayout;
	// Current temperature layout
	QGraphicsLinearLayout* iHorizontalCurrentTempLayout;
	QGraphicsLinearLayout* iVerticalLocationLayout;
	QGraphicsAnchorLayout* iAnchorLayout;
	QGraphicsLinearLayout* iHorizontalForecastLayout;
	QGraphicsLinearLayout* iHorizontalRibbonLayout;
	
	// Location info 
	QList<LocationInfoT> iLocationList;
	QList<LocationInfoT> iUserSelectedLocationList;
	SelectLocation* iSelectLocation;
	
	// Weather Info 
	WeatherInfoT iCurrentWeatherInfo;
	ConditionImageDetailsT iCurrentConditionImageDetails;
	QList<ForecastInfoT> iForecastList;
	LocationInfoT iCurrentLocationInfo;

	// Current Location widgets
	WeatherGraphicsWidget* iCurrentLocation;
	WeatherGraphicsWidget* iCurrentLocationLevel2;
	WeatherGraphicsWidget* iCurrentLocationLevel3;

	// Temperature widgets
	WeatherGraphicsWidget* iCurrentTemperature;
	WeatherGraphicsWidget* iCurrentTemperatureHighLow;
	WeatherGraphicsWidget* iCurrentTemperatureUnits;
	WeatherGraphicsWidget* iCurrentCondition;

	// Forecast widgets
	QList<WeatherForecastWidget*> iForecastWidgetsList;
	WeatherDialog* iForecastDialog;
	
	// buttons
	GraphicsWidgetItem* iCloseButton;
	GraphicsWidgetItem* iMinimizeButton;
	GraphicsWidgetItem* iSettingsButton;
	GraphicsWidgetItem* iHelpButton;
	GraphicsWidgetItem* iFavoritesButton;
	GraphicsWidgetItem* iMapsButton;
	//GraphicsWidgetItem* iStatusText;
	WeatherDialog*      iStatusDialog;
	
	// pixmap list used to hold conditon icons
	QList<QGraphicsPixmapItem*> iGraphicsPixmapItemList;
	QList<QPixmap> iMappedPixmapList;
};
#endif /*WEATHERGRAPHICSWINDOW_H_*/
