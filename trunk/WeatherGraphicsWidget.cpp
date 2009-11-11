
#include "WeatherGraphicsWidget.h"
#include <QGraphicsProxyWidget>

//#define _SHOW_FUNKY_BAR

WeatherGraphicsWidget::WeatherGraphicsWidget(QGraphicsWidget* aParent)
{
	iProxyWidget = new QGraphicsProxyWidget(this);
	setZValue(KLowestZOrder);
	// Let this item be deleted by QGraphicsItem 
	setOwnedByLayout(false);
}


WeatherGraphicsWidget::~WeatherGraphicsWidget()
{
qDebug()<<"WeatherGraphicsWidget::~WeatherGraphicsWidget->";
	if(iProxyWidget)
	{
		delete iProxyWidget;
	}
qDebug()<<"WeatherGraphicsWidget::~WeatherGraphicsWidget<-";
}


void WeatherGraphicsWidget::updateText(const QString& aText)
{
	iTextToDisplay = aText;
	update();
}

void WeatherGraphicsWidget::setText(const QString& aText,
									QFont aFont,
									int aAlignment)
{
	iTextToDisplay = aText;
	iFont = aFont;
	iAlignment = aAlignment;
	update();
}

void WeatherGraphicsWidget::setFont(QFont aFont)
{
	iFont = aFont;
	update();
}

void WeatherGraphicsWidget::setEmbedWidget(QWidget* aEmbedWidget)
{
	aEmbedWidget->setVisible(false);
	iProxyWidget->setWidget(aEmbedWidget);
}

void WeatherGraphicsWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           						  QWidget *widget)
{
	calculatePreferredGeometry(iTextToDisplay,iFont);
	qDebug()<<iTextToDisplay<<" w:"<<this->rect().width()<<" h:"<<this->rect().height();

	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setFont(iFont);

	qDebug()<<" from rect: "<<iTextToDisplay<<" w:"<<rect().toAlignedRect().width()<<" h:"<<rect().toAlignedRect().height();
	
#ifdef _SHOW_FUNKY_BAR	
	// Do not resize the bounding rect of text. 
	// This happens to alter the mouse click event due to change in geometry 
	// Just resize and center the funky bar
	
	// TODO: try to move this code to calculatePreferredGeometry
	// create rect for funky bar
	//
	QRectF funkyrect = this->rect();
	QPointF originalcenter = rect().center();
	
	// resize to required size
	funkyrect.setHeight(2*iHeight);
	funkyrect.setWidth(2*iWidth);
	
	// now move the resized rect to original center of paint area
	funkyrect.moveCenter(originalcenter);
	
	// draw funky rect
	painter->drawRect(funkyrect);
#endif
	
	painter->setPen(QColor(Qt::black));
	painter->drawText(rect().toAlignedRect(),iAlignment,iTextToDisplay);
	painter->restore();
	
}

void WeatherGraphicsWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit itemExecuted();	
}

void WeatherGraphicsWidget::calculatePreferredGeometry(QString aText,QFont aFont)
{

	QFontMetrics fontmetrics(aFont);
	iWidth = fontmetrics.width(aText);
	iHeight = fontmetrics.height();
	qDebug()<<"WeatherGraphicsWidget::calculatePreferredGeometry "<<iTextToDisplay<<" w:"<<iWidth<<" h:"<<iHeight;
}

// eof
