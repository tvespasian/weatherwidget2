
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
	//iProxyWidget->show();
	calculatePreferredGeometry(iTextToDisplay,iFont);
	qDebug()<<iTextToDisplay<<" w:"<<this->rect().width()<<" h:"<<this->rect().height();

	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setFont(iFont);
	
	QPointF center = rect().center();

	this->resize(2*iWidth,2*iHeight);

	QRectF newrect = this->rect();
	newrect.moveCenter(center);
	
	this->setGeometry(newrect);
	this->updateGeometry();
	
	qDebug()<<" from rect: "<<iTextToDisplay<<" w:"<<rect().toAlignedRect().width()<<" h:"<<rect().toAlignedRect().height();
	
	//painter->setPen(QColor(Qt::red));
	//painter->drawRect(newrect);
	
	painter->setOpacity(qreal(0.5));
	
	painter->setPen(iColor);
	painter->setBrush(QBrush(iColor));
	painter->drawRect(newrect);
	
	painter->setPen(QColor(Qt::black));
	painter->setOpacity(qreal(1.0));
	painter->drawText(newrect,iAlignment,iTextToDisplay);
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
