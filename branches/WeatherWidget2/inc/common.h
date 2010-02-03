#ifndef COMMON_H_
#define COMMON_H_

#include <QtCore>
#include <QPixmap>

// setting macros
//#define MOBILE_SCREEN

// enable this to draw bounding rect
//#define DEBUG_DRAWING

const int KStrechFactorPercent(40);
#ifndef MOBILE_SCREEN
const int KWindowWidth(400);
const int KWindowHeight(400);
const int KExtraStrech((KStrechFactorPercent*KWindowHeight)/100);
const int KMaxForecastItems(4); 
#else
const int KWindowWidth(240);
const int KWindowHeight(320);
const int KExtraStrech((KStrechFactorPercent*KWindowHeight)/100);
const int KMaxForecastItems(4); 
#endif

const QString KProductName("WW2");

/************** XML ***************/

const QString KElementLatitute("latitude");
const QString KElementLongitude("longitude");
const QString KElementRainToday("rain-today"); 
const QString KElementTemp("temp");
const QString KElementTempHigh("temp-high");
const QString KElementTempLow("temp-low");
const QString KElementAvgWindSpeed("wind-speed-avg");
const QString KElementAvgWindDirection("wind-direction-avg");
const QString KElementCurrentCondition("current-condition");
const QString KElementMoonphase("moon-phase");
const QString KElementObservationDate("ob-date");

// DateTime
const QString KElementYear("year");
const QString KElementMonth("month");
const QString KElementDay("day");
const QString KElementHour("hour");
const QString KElementMinute("minute");
const QString KElementSecond("second");
const QString KElementAM_PM("am-pm");

// Sunrise / Sunset
const QString KElementSunrise("sunrise");
const QString KElementSunset("sunset");

const QString KAttributeNumber("number");
const QString KAttribteAbbrevation("abbrv");

// Moonphase
const QString KAttributeMoonPhaseImage("moon-phase-img");

// Location
const QString KElementStation("station");
const QString KElementLocation("location");
const QString KAttributeCityName("cityname");
const QString KAttributeCountryName("countryname");
const QString KAttributeStateName("statename");
const QString KAttributeZipCode("zipcode");
const QString KAttributeCityCode("citycode");
const QString KAttributeCity("city");

// forecast
const QString KElementForecast("forecast");
const QString KElementTitle("title");
const QString KElementShortPrediction("short-prediction");
const QString KElementPrediction("prediction");
const QString KElementHigh("high");
const QString KElementLow("low");
const QString KElementImage("image");
const QString KAtrributeAlttitle("alttitle");
const QString KAttributeIsNight("isNight");
const QString KAttributeIcon("icon");

 
const QString KAm("AM");

// GeoNames.org
const QString KElementTime("time");

//Image Mapping
const QString KIMElementImage("image");
const QString KIMElementLayers("layers");
const QString KIMMappedImage("mappedimage");
const QString KIMAttributeWBImage("wbimage"); // weatherbug image
const QString KIMAttributeDayImage("dayimage"); 

/************** XML ***************/

// Setting keys
const QString KLocationInfoGroup("locationinfo");
const QString KCityName("cityname");
const QString KStateName("statename");
const QString KCountryName("countryname");
const QString KCityCode("citycode");
const QString KZipCode("zipcode");

const QString KPreferencesGroup("preferences");
const QString KUnits("units");
const QString KTimeout("timeout");

struct LocationInfoT
{
	QString iCityName;
	QString iStateName;
	QString iCountryName;
	// Note: For american loactions zipcode is set instead
	QString iCityCode;
	QString iZipCode;
};

struct WeatherInfoT
{
	QString iCurrentTemperature;
	QString iUnits;
	QString iCurrentCondition;
	QString iCurrentConditionImage;
	QString iHigh;
	QString iLow;
	QString iRainTodayInMm;
	QString iWindSpeedKmh;
	QString iWindDirection;
	QString iMoonphaseInPercentage;
	QString iMoonPhaseImage;
	QString iLatitude; // geo details may not be filled
	QString iLongitude;
	//bool    iIsAM; // AM or PM
};

struct ForecastInfoT
{
	QString iDayShortDescription; //SUN,MON etc
	QString iDayDescription; // Sunday, Monday
	QString iLow;
	QString iHigh;
	QString iShortPrediction;
	QString iExtendedPrediction;
	QString iIcon;
	QList<QPixmap> iConditionPixmapList;
};


struct ConditionImageDetailsT
{
	// Total image layers
	int         iLayers;
	
	// If this is set, then the first image is day image, i.e., an image of sun
	// Calling client has to check whether it is day or night and depending on it should load the first image
	// For ex: If local time if 10 PM, even if this value is set, client should ignore loading first image in the list
	// Hence use it when the local time is day.
	bool        iDayImage;
	
	// Image list
	QStringList iImagesList;
};

// Preferences
struct WeatherWidgetPreferences
{
	QString iUnits;
	int 	iTimeout;
};

const int KDegSymbol = 176; // degree symbol
const QString KCelcius("C");
const QString KFahrenheit("F");

const QString KSignalMapperObjectName("signalmapper");

const QString KMappedImagesRoot(":/resources/images/mappedimages/");
const QString KMoonphasesImagesRoot(":/resources/images/moonphases/");
const QString KBackgroundImagesRoot(":/resources/images/backgrounds/");
const QString KImageMappingXMLPath(":/resources/imagemapping.xml");

const QString DefaultFontName("Arial");
const int KHighestZValue(4);

#endif /*COMMON_H_*/
