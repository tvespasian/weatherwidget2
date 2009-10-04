#ifndef WEATHERWIDGETXMLENGINE_H_
#define WEATHERWIDGETXMLENGINE_H_

#include <QtCore>

/************** XML ***************/
const QString KElementRainToday("rain-today");
const QString KElementTemp("temp");
const QString KElementTempHigh("temp-high");
const QString KElementTempLow("temp-low");
const QString KElementAvgWindSpeed("wind-speed-avg");
const QString KElementAvgWindDirection("wind-direction-avg");
const QString KElementCurrentCondition("current-condition");
const QString KElementStation("station");
const QString KElementLocation("location");
const QString KAttributeCityName("cityname");
const QString KAttributeCountryName("countryname");
const QString KAttributeStateName("statename");
const QString KAttributeZipCode("zipcode");
const QString KAttributeCityCode("citycode");
const QString KAttributeCity("city");
/************** XML ***************/

struct LocationInfoT
{
	QString iCityName;
	QString iStateName;
	QString iCountryName;
	QString iCityCode;
	QString iZipCode;
	QString iDisplayString;
};

struct WeatherInfoT
{
	QString iCurrentTemperature;	
	QString iCurrentCondition;
	QString iCurrentConditionIconUrl;
	QString iRainTodayInMm;
	QString iWindSpeedKmh;
	QString iWindDirection;
	QString iGustSpeed;
	QString iGustDirection;
	QString iLatitude; // geo details may not be filled
	QString iLongitude;
};


class WeatherWidgetXMLEngine:public QObject
{
	Q_OBJECT
public:
	static bool getLocationDetails(const QString& aXMLData,QList<LocationInfoT>& aLocationInfo);
	static bool getWeatherInfo(const QString& aXMLData,WeatherInfoT& aWeatherInfo);
};

#endif /*WEATHERWIDGETXMLENGINE_H_*/

// eof