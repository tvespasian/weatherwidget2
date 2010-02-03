#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "inc\WeatherDialog.h"
#include "inc\common.h"

WeatherDialog::WeatherDialog(const QSize& aSize,
							 QGraphicsItem* aParent)
			  :QGraphicsWidget(aParent),
			  iCloseOnClick(false)
			  
{
	setZValue(KHighestZValue);
	setVisible(false);
	resize(aSize);

	iHeadingTextItem = new QGraphicsTextItem(this);
	iHeadingTextItem->setTextWidth(aSize.width()-10);
	iHeadingTextItem->setPos(QPointF(0,this->rect().height()-10));
	iHeadingTextItem->setFont(QFont(DefaultFontName,(6*(rect().width()/2))/100,QFont::Bold));
	iHeadingTextItem->setDefaultTextColor(QColor(Qt::blue));
	
	iBodyTextItem = new QGraphicsTextItem(this);
	iBodyTextItem->setTextWidth(aSize.width()-10);
	iBodyTextItem->setFont(QFont(DefaultFontName,(5*(rect().width()/2))/100));
	iBodyTextItem->setDefaultTextColor(QColor(Qt::white));
}

WeatherDialog::~WeatherDialog()
{
// left blank	
}

void WeatherDialog::displayTextWidget(bool aShow,const QStringList& aStringList)
{
	setVisible(false);
	int listcount = aStringList.count();
	if(aShow && listcount)
	{
		iHeadingTextItem->setHtml(aStringList[0]);
		
		iBodyTextItem->setHtml("");
		if(listcount >= 2){
		iBodyTextItem->setHtml(aStringList[1]);}
		
		resize(QSizeF(size().width(),iHeadingTextItem->boundingRect().height()+
 					 				  iBodyTextItem->boundingRect().height()));

		iHeadingTextItem->setPos(0,0);
		iBodyTextItem->setPos(0,iHeadingTextItem->boundingRect().height());

		// move widget to center 
		this->setPos(parentItem()->boundingRect().center().x() - rect().width()/2,
					 parentItem()->boundingRect().height()/2 - rect().height());
		animateAngle(0,360);
	}
}

void WeatherDialog::paint(QPainter *painter, 
		   				  const QStyleOptionGraphicsItem *option,
		   				  QWidget *widget)
{
	// draw to this painter path
	QPainterPath rounded_rect;
	rounded_rect.addRoundRect(rect(),20);
	painter->setOpacity(0.89);
	painter->fillPath(rounded_rect,QBrush(Qt::black));
	painter->setOpacity(1);
}

void WeatherDialog::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(!iCloseOnClick)
	{
		return;
	}
	
	setVisible(false);
	event->accept();
}

void WeatherDialog::setRotationAngleY(qreal angle)
{
		setVisible(true);
        QPointF c = boundingRect().center();
        QTransform t;
        t.translate(c.x(), c.y());
        t.rotate(angle, Qt::YAxis);
        c = boundingRect().center();
        t.translate(-c.x(), -c.y());
        setTransform(t);
}

qreal WeatherDialog::rotationAngleY() const
{
    return 0;
}

void WeatherDialog::animateAngle(int startAngle, int endAngle)
{
    // Start animate this class
    QPropertyAnimation* anim = new QPropertyAnimation(this, "rotationAngleY");

    // 2 second duration animation
    anim->setDuration(1000);
    // angle to start animation
    anim->setStartValue(startAngle);
    // end angle of animation
    anim->setEndValue(endAngle);
    // easing curve
    anim->setEasingCurve(QEasingCurve::OutQuad);

    // Listen animation finished signal
    //QObject::connect(anim, SIGNAL(finished()), this, SLOT(animationFinished()));

    // Start animation and delete QPropertyAnimation class on the end
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}


// end of file

