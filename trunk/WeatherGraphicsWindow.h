
// This is highlevel window for weather info

#ifndef WEATHERGRAPHICSWINDOW_H_
#define WEATHERGRAPHICSWINDOW_H_

#include <QGraphicsWidget>
#include <QSignalMapper>

#include "WeatherGraphicsWidget.h"
#include "WeatherWidgetNetworkEngine.h"
#include "WeatherWidgetXMLEngine.h"

const QString KSignalMapperObjectName("signalmapper");

// This is a widget to hold the layout. So no paint is required
//
class WeatherGraphicsWindow:public QGraphicsWidget
{
	Q_OBJECT
	
public:
	WeatherGraphicsWindow(QObject* aParent=0);
	~WeatherGraphicsWindow();
	
private:
	void createWeatherItems();
	void startWeatherDataUpdate();
	
private	slots:
	// QSignalMapper
	void on_signalmapper_mapped(int);

	// WeatherWidgetNetworkEngine
	void on_networkEngine_weatherRequestCompleted(const WeatherResponseT& aResponse,
												  RequestMethodT aRequestMethod,
											      bool aError);
private:
	QSignalMapper* iSignalMapper;
	WeatherWidgetNetworkEngine* iNetworkEngine;
	LocationInfoT iCurrentLocationInfo;
	WeatherInfoT iCurrentWeatherInfo;
private:
	
	// This is the high level layout for the view
	QGraphicsLinearLayout* iVerticalViewLayout;
	
	// Container layout for current temperature
	QGraphicsLinearLayout* iVerticalCurrentTempContainer;
	
	// Current temperature layout
	QGraphicsLinearLayout* iHorizontalCurrentTempLayout;
	
	// Container layout for forecast
	QGraphicsLinearLayout* iHorizontalForecastLayoutContainer;

	// Current Location
	WeatherGraphicsWidget* iCurrentLocation;
	
	// Current Temperature
	WeatherGraphicsWidget* iCurrentTemperature;
	
	// Celcius or F
	WeatherGraphicsWidget* iCurrentTemperatureUnits;
	
	// Current description of the condition
	WeatherGraphicsWidget* iCurrentCondition;
	
/* Current Temperature Values */
	
	
/* Forecast Temperature Values */
	
	
/* Forecast Temperature Values */

};
#endif /*WEATHERGRAPHICSWINDOW_H_*/
