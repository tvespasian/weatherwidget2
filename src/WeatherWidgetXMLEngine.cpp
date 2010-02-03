
#include "inc\WeatherWidgetXMLEngine.h"
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
			QString xmlTagName = xml.name().toString();
			qDebug()<<xmlTagName;
			
			// current temp
			if(xmlTagName == KElementTemp)
			{
				aWeatherInfo.iCurrentTemperature = xml.readElementText();
				isfound = true;	
			}
			
			// latitude
			else if(xmlTagName == KElementLatitute)
			{
				aWeatherInfo.iLatitude = xml.readElementText();
			}
			
			// longitude
			else if(xmlTagName == KElementLongitude)
			{
				aWeatherInfo.iLongitude = xml.readElementText();
			}

			// current condition
			else if(xmlTagName == KElementCurrentCondition)
			{
				QFileInfo fileinfo(xml.attributes().value(KAttributeIcon).toString());
				aWeatherInfo.iCurrentConditionImage = fileinfo.baseName(); 
				aWeatherInfo.iCurrentCondition = xml.readElementText();
				qDebug()<<"condition icon:"<<aWeatherInfo.iCurrentConditionImage;
			}
			
			// High Temperature
			else if(xmlTagName == KElementTempHigh)
			{
				aWeatherInfo.iHigh = xml.readElementText();
			}
			
			// Low Temperature
			else if(xmlTagName == KElementTempLow)
			{
				aWeatherInfo.iLow = xml.readElementText();
			}
			
			// Rain today
			else if(KElementRainToday == xmlTagName)
			{
				aWeatherInfo.iRainTodayInMm = xml.readElementText();
			}
			
			// Windspeed-Avg
			else if(KElementAvgWindSpeed == xmlTagName)
			{
				aWeatherInfo.iWindSpeedKmh = xml.readElementText();
			}
			
			// Wind direction-Avg
			else if(KElementAvgWindDirection == xmlTagName)
			{
				aWeatherInfo.iWindDirection = xml.readElementText();
			}
	
			// Moonphase
			else if(KElementMoonphase == xmlTagName)
			{
				aWeatherInfo.iMoonPhaseImage = xml.attributes().value(KAttributeMoonPhaseImage).toString();
				aWeatherInfo.iMoonphaseInPercentage = xml.readElementText();
				QFileInfo fileinfo(aWeatherInfo.iMoonPhaseImage);
				aWeatherInfo.iMoonPhaseImage = fileinfo.baseName().right(2);
			}

#ifdef xx			
			// observation time
			else if(KElementObservationDate == xmlTagName)
			{
				qDebug()<<"WeatherWidgetXMLEngine::getWeatherInfo found observation date tag";
				bool exit = false;
				do
				{
					xml.readNext();
					xmlTagName = xml.name().toString();
					qDebug()<<"WeatherWidgetXMLEngine::getWeatherInfo "<<xmlTagName;

					if(KElementObservationDate == xmlTagName && xml.isEndElement())
					{
						qDebug()<<"WeatherWidgetXMLEngine::getWeatherInfo end element";
						exit = true;	
					}
					
					else if(KElementAM_PM == xmlTagName)
					{
						QString tmp = xml.attributes().value(KAttribteAbbrevation).toString();
						qDebug()<<"WeatherWidgetXMLEngine::getWeatherInfo am_pm"<<tmp;
						
						if(tmp == KAm)
						{
							aWeatherInfo.iIsAM = true;
						}
						else
						{
							aWeatherInfo.iIsAM = true;
						}
					}
				}while(!exit);
				qDebug()<<"WeatherWidgetXMLEngine::getWeatherInfo result "<<aWeatherInfo.iIsAM;
				
			} // observation time
#endif			
		} //if(xml.isStartElement())	
	} // while

qDebug()<<"WeatherWidgetXMLEngine::getWeatherInfo<-";
return isfound;
}

void WeatherWidgetXMLEngine::getForecastInfo(const QString& aXMLData,QList<ForecastInfoT>& aForecastList)
{
qDebug()<<"WeatherWidgetXMLEngine::getForecastInfo->";	
	int index = -1;
	aForecastList.clear();
	QXmlStreamReader xml;
	xml.addData(aXMLData);
	
	while(!xml.atEnd())
	{
		xml.readNext();
		if(xml.isStartElement())
		{
			QString xmlTagName = xml.name().toString();
			qDebug()<<xmlTagName;

			// forecast for a day starts
			if(KElementForecast == xmlTagName)
			{
				// add element to list to hold data to be read
				ForecastInfoT forecast;
				aForecastList.append(forecast);
				index++;
				qDebug()<<"Adding forecast element:"<<index;
			}
			
			// Forecast elements of a day starts
			if(KElementTitle == xmlTagName)
			{
				aForecastList[index].iDayShortDescription = xml.attributes().value(KAtrributeAlttitle).toString();
				aForecastList[index].iDayDescription = xml.readElementText();	
			}
			
			else if(KElementShortPrediction == xmlTagName)
			{
				aForecastList[index].iShortPrediction = xml.readElementText();
			}
			
			else if(KElementPrediction == xmlTagName)
			{
				aForecastList[index].iExtendedPrediction = xml.readElementText();
			}
			
			else if(KElementHigh == xmlTagName)
			{
				aForecastList[index].iHigh = xml.readElementText();
			}
			
			else if(KElementLow == xmlTagName)
			{
				aForecastList[index].iLow = xml.readElementText();
			}
			
			else if(KElementImage == xmlTagName)
			{
				aForecastList[index].iIcon = QFileInfo(xml.attributes().value(KAttributeIcon).toString()).baseName().right(3);
			}
		}
	} // while
	
qDebug()<<"WeatherWidgetXMLEngine::getForecastInfo<-";
}


bool WeatherWidgetXMLEngine::getLocalTime(const QString& aXMLData,QDateTime& aDateTime)
{
qDebug()<<"WeatherWidgetXMLEngine::getLocalTime->";

bool found = false;
//2010-01-19 01:17
QString datetimeformat("yyyy-MM-dd hh:mm");
QXmlStreamReader xml;
xml.addData(aXMLData);

	while(!xml.atEnd())
	{
		xml.readNext();
		if(xml.isStartElement())
		{
			// forecast for a day starts
			if(KElementTime == xml.name().toString())
			{
				// read content
				QString datetimestring = xml.readElementText();
				if(!datetimestring.isEmpty())
				{
					found = true;
					aDateTime = QDateTime::fromString(datetimestring,datetimeformat);
				}
			}
		}
	} // while
qDebug()<<"WeatherWidgetXMLEngine::getLocalTime<-";
return found;
}


bool WeatherWidgetXMLEngine::getMappedCondtionImageDetails(const QStringRef& aImageIdToMap,
														   const QString& aXMLData,
														   ConditionImageDetailsT& aConditionImageDetails)

{
qDebug()<<"WeatherWidgetXMLEngine::getMappedCondtionImageDetails->";
	bool isFound = false;
	aConditionImageDetails.iDayImage = false;
	QString imageids;
	QXmlStreamReader xml;
	xml.addData(aXMLData);
	
	while(!xml.atEnd())
	{
		xml.readNext();
		if(xml.isStartElement())
		{
			// found image
			if(KIMElementImage == xml.name().toString())
			{
				// break parsing
				if(isFound)
				{
					break;
				}
				// get wbimages
				imageids.clear();
				imageids += xml.attributes().value(KIMAttributeWBImage);
				
				// search for required image
				if(imageids.contains(aImageIdToMap.toString()))
				{
				isFound = true;
				}
			}
			
			else if(isFound && KIMElementLayers == xml.name().toString())
			{
				aConditionImageDetails.iLayers = xml.readElementText().toInt();
			}
			
			else if(isFound && KIMMappedImage == xml.name().toString())
			{
				// get whether a dayimage is present
				if(xml.attributes().hasAttribute(KIMAttributeDayImage))
				{
					qDebug()<<"XML Parsing dayimage:"<<xml.attributes().value(KIMAttributeDayImage).toString();
					("true" == xml.attributes().value(KIMAttributeDayImage).toString()) ? 
							(aConditionImageDetails.iDayImage = true):
									(aConditionImageDetails.iDayImage = false);
				}
				
				// get image
				aConditionImageDetails.iImagesList.append(xml.readElementText());
			}
		}
	}

qDebug()<<"WeatherWidgetXMLEngine::getMappedCondtionImageDetails<-";	
return isFound;
}
// eof