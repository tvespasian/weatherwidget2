
#include "inc\WeatherGraphicsWidget.h"
#include <QGraphicsProxyWidget>

WeatherGraphicsWidget::WeatherGraphicsWidget(const QString& aText,
											 const QFont& aFont,
											 const int& aAlignment,
											 QColor aColor,
											 QGraphicsWidget* aParent)
					  :QGraphicsWidget(aParent),
					  iFont(aFont),
					  iAlignment(aAlignment),
					  iColor(aColor),
					  iTextToDisplay(aText)
{
	setOwnedByLayout(true);
	setText(aText);
}

WeatherGraphicsWidget::~WeatherGraphicsWidget()
{
qDebug()<<"WeatherGraphicsWidget::~WeatherGraphicsWidget->";
qDebug()<<"WeatherGraphicsWidget::~WeatherGraphicsWidget<-";
}

void WeatherGraphicsWidget::setText(const QString& aText)
{
	iTextToDisplay = aText;
	modifyFontSizeIfRequired();
}

void WeatherGraphicsWidget::setFont(const QFont& aFont,
									const QColor& aColor)
{
	iFont = aFont;
	iColor = aColor;
	modifyFontSizeIfRequired();
}

void WeatherGraphicsWidget::paint(QPainter *painter, 
								  const QStyleOptionGraphicsItem *option,
           						  QWidget *widget)
{
//qDebug()<<"WeatherGraphicsWidget::paint->";

	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | 
	    			   		QPainter::SmoothPixmapTransform | 
	    			   		QPainter::TextAntialiasing);

#ifdef DEBUG_DRAWING	
	painter->setPen(QColor(Qt::red));
	painter->drawRect(rect().toAlignedRect());
#endif
	
	QPen p(iColor);
	painter->setPen(p);
	
	painter->setFont(iFont);
	painter->drawText(rect().toAlignedRect(),iAlignment,iTextToDisplay);
	
	painter->restore();
	
//qDebug()<<"WeatherGraphicsWidget::paint<-";	
}

void WeatherGraphicsWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit itemExecuted();	
}

void WeatherGraphicsWidget::modifyFontSizeIfRequired()
{
#ifdef MODIFYSIZE	
	QFontMetrics metrics(iFont);
	int textwidth = metrics.boundingRect(iTextToDisplay).width() + 30; // adding buffer
	int boundingRectWidth = boundingRect().width();
	
	if(textwidth && textwidth > boundingRectWidth) 
	{
		// need to recalculate font size to fit the text in widget boundry
		iFont.setPointSize(((boundingRectWidth)*iFont.pointSize())/textwidth); 
	}
#endif	
}

int WeatherGraphicsWidget::getTextWidth()
{
	QFontMetrics fm(iFont);
	return fm.width(iTextToDisplay);
}

int WeatherGraphicsWidget::getTextHeight()
{
	QFontMetrics fm(iFont);
	return fm.height();
}

















// eof
