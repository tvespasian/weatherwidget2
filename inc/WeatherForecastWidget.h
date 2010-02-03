#ifndef WEATHERFORECASTWIDGET_H_
#define WEATHERFORECASTWIDGET_H_

#include <QGraphicsWidget>
#include "WeatherDialog.h"
#include "common.h"

class WeatherForecastWidget:public QGraphicsWidget
{
	Q_OBJECT

	// For property animation
    Q_PROPERTY(qreal rotationAngleY READ rotationAngleY WRITE setRotationAngleY)
	//Q_PROPERTY(qreal rotationAngleY READ rotationAngleY WRITE setRotationAngleY)

public:
	WeatherForecastWidget(QGraphicsWidget* aParent=0);
	~WeatherForecastWidget();
	
public:
	void setForecastInfo(const ForecastInfoT& aForecastInfo);
	
	// updated only temperature
	// used for updating C to F or vice versa
	void updateTemperature(const ForecastInfoT& aForecastInfo);
	
// Properties	
public:
    // For rotationAngleY property
    void setRotationAngleY(qreal angle);
    qreal rotationAngleY() const{return 0;}

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event);
	
signals:
	void displayDetailedForecast(bool aShow,const QStringList& aStringList);
	
protected:
    // Animate angle of this class
    void animateAngle(int startAngle, int endAngle);

private:
	// Only one forecast item can be highlighted
	// This synchronizes highlight among all forecast items
	static QGraphicsPathItem* iHighlightPathItem;
	static QGraphicsPathItem* getHighlightItem(){return iHighlightPathItem;}
	
	void alignToCenter(QGraphicsTextItem*& aTextItem,int aY = 0);
	
	//ForecastInfoT iForecastInfo;
	QGraphicsTextItem* iTextItemLevel1;
	QGraphicsTextItem* iTextItemLevel2;
	QGraphicsItem* iParentItem;
	QList<QGraphicsPixmapItem*> iPixmapList;
	QStringList iDescriptionList;
	bool iIsHighlighted;
};
#endif /*WEATHERFORECASTWIDGET_H_*/
