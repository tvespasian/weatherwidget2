
#include "WeatherGraphicsWidget.h"
#include <QGraphicsProxyWidget>

WeatherGraphicsWidget::WeatherGraphicsWidget(QGraphicsWidget* aParent)
{
	iProxyWidget = new QGraphicsProxyWidget(this);
	
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
//	calculatePreferredGeometry(iTextToDisplay,iFont);
	
	update();
}

void WeatherGraphicsWidget::setFont(QFont aFont)
{
	iFont = aFont;
//	calculatePreferredGeometry(iTextToDisplay,iFont);
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
	//iProxyWidget->show();
//	calculatePreferredGeometry(iTextToDisplay,iFont);
	qDebug()<<iTextToDisplay<<" w:"<<this->rect().width()<<" h:"<<this->rect().height();

	painter->save();
	painter->setFont(iFont);
	
	qDebug()<<" from rect: "<<iTextToDisplay<<" w:"<<rect().toAlignedRect().width()<<" h:"<<rect().toAlignedRect().height();
	
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
	/*setPreferredHeight(qreal(iHeight));
	setPreferredWidth(qreal(iWidth));
	adjustSize();*/
}

// eof
