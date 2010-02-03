
#include <QtGui>
#include "inc\WeatherForecastWidget.h"
//bool WeatherForecastWidget::iIsHighlighted = false;
QGraphicsPathItem* WeatherForecastWidget::iHighlightPathItem = 0;

WeatherForecastWidget::WeatherForecastWidget(QGraphicsWidget* aParent)
					  :QGraphicsWidget(aParent),
					   iIsHighlighted(false)
{
	setObjectName(QTime::currentTime().toString());
	iParentItem = 0;
	iTextItemLevel1 = new QGraphicsTextItem(this);
	iTextItemLevel1->setFont(QFont(DefaultFontName,(2*(rect().width()/2))/100));
	iTextItemLevel1->setDefaultTextColor(Qt::white);
	
	iTextItemLevel2 = new QGraphicsTextItem(this);
	iTextItemLevel2->setFont(QFont(DefaultFontName,(6*(rect().width()/2))/100,QFont::Bold));
	iTextItemLevel2->setDefaultTextColor(Qt::white);
}

WeatherForecastWidget::~WeatherForecastWidget()
{
// left blank	
}

void WeatherForecastWidget::setForecastInfo(const ForecastInfoT& aForecastInfo)
{
	qDebug()<<"WeatherForecastWidget::setForecastInfo "<<aForecastInfo.iIcon;
	iTextItemLevel1->setPlainText(aForecastInfo.iDayDescription);
	alignToCenter(iTextItemLevel1);

	(0 == iParentItem)?(delete iParentItem):(iParentItem->setVisible(false));
	iParentItem = new QGraphicsPixmapItem(this);
	
	// pixmap
	for(int i=0;i<aForecastInfo.iConditionPixmapList.count();i++)
	{
		
		QPixmap pixmapToLoad = QPixmap(aForecastInfo.iConditionPixmapList[i]).scaled(rect().height()/2,
																			  rect().height()/2,
																			  Qt::IgnoreAspectRatio,
																			  Qt::SmoothTransformation);
		
		QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(pixmapToLoad,iParentItem);
		//iPixmapList.append(pixmapItem);
		pixmapItem->setPos(QPointF(rect().center().x()-pixmapToLoad.width()/2,
					  		       rect().center().y()- (pixmapToLoad.height()/2 
					  		       - ((0==i)?(0):(i*5)))));
	}
	updateTemperature(aForecastInfo);
	
	iDescriptionList.clear();
	iDescriptionList.append(aForecastInfo.iShortPrediction);
	iDescriptionList.append(aForecastInfo.iExtendedPrediction);
}

void WeatherForecastWidget::updateTemperature(const ForecastInfoT& aForecastInfo)
{
	iTextItemLevel2->setPlainText(aForecastInfo.iHigh+"/"+
								  aForecastInfo.iLow);

	alignToCenter(iTextItemLevel2,rect().height()-25);
}

void WeatherForecastWidget::alignToCenter(QGraphicsTextItem*& aTextItem,int aY)
{
	QPointF textpos(rect().center().x() - aTextItem->boundingRect().width()/2,aY);
	aTextItem->setPos(textpos);
}

void WeatherForecastWidget::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	// Create item if not exists
	if(0 == iHighlightPathItem)
	{
		iHighlightPathItem = new QGraphicsPathItem;
		iHighlightPathItem->setVisible(false);
	}
	
	// highlight is not for this item
	if(!iIsHighlighted)
	{
		iHighlightPathItem->setVisible(false);
		iHighlightPathItem->setParentItem(this);
		
		QPainterPath rounded_rect;
		rounded_rect.addRoundRect(rect(),20);
		iHighlightPathItem->setPath(rounded_rect);
	}
	emit displayDetailedForecast(!iHighlightPathItem->isVisible(),iDescriptionList);
	iHighlightPathItem->setVisible(!iHighlightPathItem->isVisible());
	iIsHighlighted = iHighlightPathItem->isVisible();

	event->accept();
}


void WeatherForecastWidget::setRotationAngleY(qreal angle)
{
        QPointF c = boundingRect().center();
        QTransform t;
        t.translate(c.x(), c.y());
        t.rotate(angle, Qt::YAxis);
        c = boundingRect().center();
        t.translate(-c.x(), -c.y());
        setTransform(t);
}


void WeatherForecastWidget::animateAngle(int startAngle, int endAngle)
{
	
}
// eof
