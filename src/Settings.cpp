
#include <QtGui>
#include "inc\Settings.h"

Settings::Settings()
{
	
}

Settings::~Settings()
{
	
}

void Settings::writeLocationSettings(const LocationInfoT& aLocationInfo)
{
	QSettings settings(QApplication::organizationName(),QApplication::applicationName());
	settings.beginGroup(KLocationInfoGroup);
	settings.setValue(KCityName,aLocationInfo.iCityName);
	settings.setValue(KStateName,aLocationInfo.iStateName);
	settings.setValue(KCountryName,aLocationInfo.iCountryName);
	settings.setValue(KCityCode,aLocationInfo.iCityCode);
	settings.setValue(KZipCode,aLocationInfo.iZipCode);
	settings.endGroup();
}

void Settings::readLocationSettings(LocationInfoT& aLocationInfo)
{
	QSettings settings(QApplication::organizationName(),QApplication::applicationName());
	settings.beginGroup(KLocationInfoGroup);
	aLocationInfo.iCityCode = settings.value(KCityCode).toString();
	aLocationInfo.iCityName = settings.value(KCityName).toString();
	aLocationInfo.iStateName = settings.value(KStateName).toString();
	aLocationInfo.iCountryName = settings.value(KCountryName).toString();
	aLocationInfo.iCityCode = settings.value(KCityCode).toString();
	aLocationInfo.iZipCode = settings.value(KZipCode).toString();
	settings.endGroup();
}

void Settings::writePreferences(const WeatherWidgetPreferences& aPreferences)
{
	QSettings settings(QApplication::organizationName(),QApplication::applicationName());
	settings.beginGroup(KPreferencesGroup);
	settings.setValue(KUnits,aPreferences.iUnits);
	settings.setValue(KTimeout,aPreferences.iTimeout);
	settings.endGroup();
}

void Settings::readPreferences(WeatherWidgetPreferences& aPreferences)
{
	QSettings settings(QApplication::organizationName(),QApplication::applicationName());
	settings.beginGroup(KPreferencesGroup);
	aPreferences.iUnits = settings.value(KUnits).toString();
	aPreferences.iTimeout = settings.value(KTimeout).toInt();
	settings.endGroup();
}

// eof
