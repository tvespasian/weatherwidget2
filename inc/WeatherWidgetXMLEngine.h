#ifndef WEATHERWIDGETXMLENGINE_H_
#define WEATHERWIDGETXMLENGINE_H_

#include <QtCore>

#include "common.h"
class WeatherWidgetXMLEngine:public QObject
{
	Q_OBJECT
public:
	static bool getLocationDetails(const QString& aXMLData,QList<LocationInfoT>& aLocationInfo);
	static bool getWeatherInfo(const QString& aXMLData,WeatherInfoT& aWeatherInfo);
	
	// client needs to check the list count
	static void getForecastInfo(const QString& aXMLData,QList<ForecastInfoT>& aForecastList);
	
	static bool getLocalTime(const QString& aXMLData,QDateTime& aDateTime);
	
	static bool getMappedCondtionImageDetails(const QStringRef& aImageIdToMap,
											  const QString& aXMLData,
											  ConditionImageDetailsT& aConditionImageDetails);
};

#endif /*WEATHERWIDGETXMLENGINE_H_*/

// eof