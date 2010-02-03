#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <QSettings>
#include "common.h"
class Settings
{
public:
	Settings();
	~Settings();
	
	void writeLocationSettings(const LocationInfoT& aLocationInfo);
	void readLocationSettings(LocationInfoT& aLocationInfo);
	
	void writePreferences(const WeatherWidgetPreferences& aPreferences);
	void readPreferences(WeatherWidgetPreferences& aPreferences);
};

#endif /*SETTINGS_H_*/
