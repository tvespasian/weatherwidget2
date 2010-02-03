#ifndef WEATHERGRAPHICSWIDGET_H_
#define WEATHERGRAPHICSWIDGET_H_

#include <QtGui>
#include <QGraphicsWidget>
#include <QGraphicsTextItem>
#include "common.h"

class WeatherGraphicsWidget:public QGraphicsWidget
{
	Q_OBJECT

public:
	WeatherGraphicsWidget(const QString& aText,
			 			  const QFont& aFont,
			 			  const int& aAlignment = Qt::AlignCenter,
			 			  QColor aColor = Qt::white,		 
			 			  QGraphicsWidget* aParent=0);
	~WeatherGraphicsWidget();
	
public:
	void setText(const QString& aText);
	inline QString getText(){return iTextToDisplay;}
	
	void setFont(const QFont& aFont,const QColor& aColor = QColor(Qt::black));
	inline QFont getFont(){return iFont;}
	
	void setColor(QColor aColor){iColor = aColor;}
	inline QColor getColor(){return iColor;}
	
	void setAlignment(int aAlignment){iAlignment = aAlignment;} 
	inline int getAlignment(){return iAlignment;}
	
	void paint(QPainter *painter, 
			   const QStyleOptionGraphicsItem *option,
	           QWidget *widget = 0);
	
	int getTextWidth();
	int getTextHeight();
	signals:
	void itemExecuted();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	
private:
	// Checks if the text with font size exceeds widget boundry
	// If so recalculates and modifies font size 
	void modifyFontSizeIfRequired();
	
private:
	QString iTextToDisplay;
	QFont iFont;
	QColor iColor;
	int iAlignment;
	int iWidth;
	int iHeight;
};

#endif /*WEATHERGRAPHICSWIDGET_H_*/
