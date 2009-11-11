#ifndef WEATHERGRAPHICSWIDGET_H_
#define WEATHERGRAPHICSWIDGET_H_

#include <QtGui>
#include <QGraphicsWidget>

const QString KDefualtFont("Arial");
const int KDefaultFontSize(12);
const qreal KToppestZOrder(1.0);
const qreal KLowestZOrder(0.5);

class QGraphicsProxyWidget;
class WeatherGraphicsWidget:public QGraphicsWidget
{
	Q_OBJECT

public:
	WeatherGraphicsWidget(QGraphicsWidget* aParent=0);
	~WeatherGraphicsWidget();
	
public:
	void updateText(const QString& aText);
	
	void setText(const QString& aText,
				 QFont aFont = QFont(KDefualtFont,KDefaultFontSize),
				 int aAlignment = Qt::AlignCenter);
	void setFont(QFont aFont);
	void setColor(QColor aColor){iColor = aColor;}
	
	void setAlignment(int aAlignment)
	{
		iAlignment = aAlignment;	
	}
	
	void setEmbedWidget(QWidget* aEmbedWidget);
	void paint(QPainter *painter, 
			   const QStyleOptionGraphicsItem *option,
	           QWidget *widget = 0);
	
	signals:
	void itemExecuted();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	
private:
	void calculatePreferredGeometry(QString aText,QFont aFont);
	
private:
	QGraphicsProxyWidget* iProxyWidget;
	QString iTextToDisplay;
	QFont iFont;
	QColor iColor;
	int iAlignment;
	int iWidth;
	int iHeight;
};

#endif /*WEATHERGRAPHICSWIDGET_H_*/
