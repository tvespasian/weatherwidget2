
#include "WeatherWidgetXMLEngine.h"
#include <QXmlStreamReader>
#include <qxmlstream.h>

bool WeatherWidgetXMLEngine::getLocationDetails(const QString& aXMLData,
	 										    QList<LocationInfoT>& aLocationInfo)
{
qDebug()<<"WeatherWidgetXMLEngine::getLocationDetails->";
qDebug()<<aXMLData;

	bool isfound = false;
	QXmlStreamReader xml;
	xml.addData(aXMLData);
	
	while(!xml.atEnd())
	{
		if(QXmlStreamReader::NoError != xml.error())
		{
			// unable to get exact error code :(
			int err = xml.error();
			return false;
		}
		
		xml.readNext();
		if(xml.isStartElement())
		{
			qDebug()<<"element: "<<xml.name().toString().toLower();
			
			// location element
			if(KElementLocation == xml.name().toString().toLower())
			{
			qDebug()<<xml.name().toString().toLower();
			LocationInfoT locationinfo;
			
			locationinfo.iCityCode = xml.attributes().value(KAttributeCityCode).toString();
			locationinfo.iCityName = xml.attributes().value(KAttributeCityName).toString();
			locationinfo.iCountryName = xml.attributes().value(KAttributeCountryName).toString();
			locationinfo.iStateName = xml.attributes().value(KAttributeStateName).toString();
			locationinfo.iZipCode = xml.attributes().value(KAttributeZipCode).toString();
				
			isfound = true;
			aLocationInfo.append(locationinfo);
			} // location element
						
		}// new element
		
	} // while
	
qDebug()<<"found locations: "<<aLocationInfo.count();	
qDebug()<<"WeatherWidgetXMLEngine::getLocationDetails<-";
return isfound;
}


bool WeatherWidgetXMLEngine::getWeatherInfo(const QString& aXMLData,WeatherInfoT& aWeatherInfo)
{
qDebug()<<"WeatherWidgetXMLEngine::getWeatherInfo->";	
	bool isfound = false;
	QXmlStreamReader xml;
	xml.addData(aXMLData);
	
	while(!xml.atEnd())
	{
	
		xml.readNext();
		if(xml.isStartElement())
		{
			qDebug()<<xml.name().toString();
			// current temp
			if(xml.name().toString() == KElementTemp)
			{
				aWeatherInfo.iCurrentTemperature = xml.readElementText();
				isfound = true;	
			}
			
			// current condition
			else if(xml.name().toString() == KElementCurrentCondition)
			{
				aWeatherInfo.iCurrentCondition = xml.readElementText();
			}
		}
		
	} // while
	
	xml.clear();	
qDebug()<<"WeatherWidgetXMLEngine::getWeatherInfo<-";
return isfound;
}

// eof