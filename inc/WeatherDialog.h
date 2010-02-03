#ifndef WEATHERDIALOG_H_
#define WEATHERDIALOG_H_

#include <QGraphicsWidget>

class WeatherDialog:public QGraphicsWidget
{
	Q_OBJECT
	
	// For property animation
    Q_PROPERTY(qreal rotationAngleY READ rotationAngleY WRITE setRotationAngleY)
	//Q_PROPERTY(qreal rotationAngleY READ rotationAngleY WRITE setRotationAngleY)

public:	
	WeatherDialog(const QSize& aSize,
				  QGraphicsItem* aParent=0);
	~WeatherDialog();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	
// Properties	
public:
    // For rotationAngleY property
    void setRotationAngleY(qreal angle);
	qreal rotationAngleY() const;
	void setCloseOnClick(bool value){iCloseOnClick = value;}
	bool closeOnClick(){return iCloseOnClick;}
public slots:
	void displayTextWidget(bool aShow,const QStringList& aStringList = QStringList());
	
protected:
	void paint(QPainter *painter, 
			   const QStyleOptionGraphicsItem *option,
	           QWidget *widget = 0);
protected:
	void animateAngle(int startAngle, int endAngle);
	
	
private:
	QGraphicsTextItem* iHeadingTextItem;
	QGraphicsTextItem* iBodyTextItem;
	bool iCloseOnClick;
};
#endif /*WEATHERDIALOG_H_*/
