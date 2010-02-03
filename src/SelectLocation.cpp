#include "inc\SelectLocation.h"
#include <QPainter>

SelectLocation::SelectLocation(QWidget *parent)
    		   :QWidget(parent)
{
	ui.setupUi(this);
}

SelectLocation::~SelectLocation()
{
//left blank
}


void SelectLocation::paintEvent(QPaintEvent *paintevent)
{
	QPainter painter;
	QPainterPath roundedrect;
	roundedrect.addRoundRect(this->rect(),20);
	
	painter.setOpacity(0.8);
	painter.fillPath(roundedrect,QColor(Qt::black));
	painter.setOpacity(1);
	
}

